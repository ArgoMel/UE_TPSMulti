// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/HitScanWeapon.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Component/LagCompensationComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

void AHitScanWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	Super::Fire(HitTargets);
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr)
	{
		return;
	}
	AController* instigatorController = ownerPawn->GetController();

	const USkeletalMeshSocket* muzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLEFLASH);
	if (muzzleFlashSocket)
	{
		const FTransform socketTransform = muzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector start = socketTransform.GetLocation();

		// Maps hit character to number of times hit
		TMap<ABaseCharacter*, uint32> hitMap;
		TMap<ABaseCharacter*, uint32> headShotHitMap;
		for (auto& hitTarget : HitTargets)
		{
			FHitResult fireHit;
			WeaponTraceHit(start, hitTarget, fireHit);

			ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(fireHit.GetActor());
			if (baseCharacter)
			{
				if (fireHit.BoneName == BONE_HEAD)
				{
					if (headShotHitMap.Contains(baseCharacter))
					{
						++headShotHitMap[baseCharacter];
					}
					else
					{
						headShotHitMap.Emplace(baseCharacter, 1);
					}
				}
				else
				{
					if (hitMap.Contains(baseCharacter))
					{
						++hitMap[baseCharacter];
					}
					else
					{
						hitMap.Emplace(baseCharacter, 1);
					}
				}

				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, fireHit.ImpactPoint, fireHit.ImpactNormal.Rotation());
				}
				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, fireHit.ImpactPoint, 0.5f, FMath::FRandRange(-0.5f, 0.5f));
				}
			}
		}
		TArray<ABaseCharacter*> hitCharacters;

		// Maps Character hit to total damage
		TMap<ABaseCharacter*, float> damageMap;

		// Calculate body shot damage by multiplying times hit x Damage - store in DamageMap
		for (auto& hitPair : hitMap)
		{
			if (hitPair.Key)
			{
				damageMap.Emplace(hitPair.Key, hitPair.Value * Damage);
				hitCharacters.AddUnique(hitPair.Key);
			}
		}

		// Calculate headshot damage by multiplying times hit x HeadShotDamage - store in DamageMap
		for (auto& headShotHitPair : headShotHitMap)
		{
			if (headShotHitPair.Key)
			{
				if (damageMap.Contains(headShotHitPair.Key))
				{
					damageMap[headShotHitPair.Key] += headShotHitPair.Value * HeadShotDamage;
				}
				else
				{
					damageMap.Emplace(headShotHitPair.Key, headShotHitPair.Value * HeadShotDamage);
				}
				hitCharacters.AddUnique(headShotHitPair.Key);
			}
		}

		// Loop through DamageMap to get total damage for each character
		for (auto& damagePair : damageMap)
		{
			if (damagePair.Key && instigatorController)
			{
				if (HasAuthority() &&
					(!bUseServerSideRewind || ownerPawn->IsLocallyControlled()))
				{
					UGameplayStatics::ApplyDamage(
						damagePair.Key, // Character that was hit
						damagePair.Value, // Damage calculated in the two for loops above
						instigatorController,
						this,
						UDamageType::StaticClass());
				}
			}
		}

		if (!HasAuthority() && bUseServerSideRewind)
		{
			if (!OwnerCharacter)
			{
				OwnerCharacter = Cast<ABaseCharacter>(ownerPawn);
			}
			if (!OwnerController)
			{
				OwnerController = Cast<ABasePlayerController>(instigatorController);
			}
			if (OwnerCharacter &&
				OwnerController &&
				OwnerCharacter->GetLagCompensation() && 
				OwnerCharacter->IsLocallyControlled())
			{
				OwnerCharacter->GetLagCompensation()->HitscanServerScoreRequest(hitCharacters, start, HitTargets, OwnerController->GetServerTime() - OwnerController->SingleTripTime);
			}
		}
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const
{
	const UWorld* world = GetWorld();
	if (!world)
	{
		return;
	}
	const FVector end = TraceStart + (HitTarget - TraceStart) * 1.25f;

	world->LineTraceSingleByChannel(OutHit, TraceStart, end, ECollisionChannel::ECC_Visibility);
	FVector beamEnd = end;
	if (OutHit.bBlockingHit)
	{
		beamEnd = OutHit.ImpactPoint;
	}
	else
	{
		OutHit.ImpactPoint = end;
	}

	if (BeamParticles)
	{
		UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(world, BeamParticles,	TraceStart, FRotator::ZeroRotator, true);
		if (beam)
		{
			beam->SetVectorParameter(FName("Target"), beamEnd);
		}
	}
}

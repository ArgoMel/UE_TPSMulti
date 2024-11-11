// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/HitScanWeapon.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
//#include "Component/LagCompensationComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr)
	{
		return;
	}

	AController* instigatorController = ownerPawn->GetController();

	const USkeletalMeshSocket* muzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLEFLASH);
	if (muzzleFlashSocket)
	{
		FTransform socketTransform = muzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector start = socketTransform.GetLocation();
		FHitResult fireHit;
		WeaponTraceHit(start, HitTarget, fireHit);

		ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(fireHit.GetActor());
		if (baseCharacter && instigatorController)
		{
			bool bCauseAuthDamage = !bUseServerSideRewind || ownerPawn->IsLocallyControlled();
			if (HasAuthority() && bCauseAuthDamage)
			{
				const float damageToCause = fireHit.BoneName == BONE_HEAD ? HeadShotDamage : Damage;

				UGameplayStatics::ApplyDamage(baseCharacter, damageToCause, instigatorController, this, UDamageType::StaticClass());
			}
			if (!HasAuthority() && bUseServerSideRewind)
			{
				if(!OwnerCharacter)
				{
					OwnerCharacter = Cast<ABaseCharacter>(ownerPawn);
				}
				if (!OwnerController)
				{
					OwnerController = Cast<ABasePlayerController>(instigatorController);
				}
				if (OwnerController && 
					OwnerCharacter && 
					//OwnerCharacter->GetLagCompensation() && 
					OwnerCharacter->IsLocallyControlled())
				{
				//	OwnerCharacter->GetLagCompensation()->ServerScoreRequest(
				//		BlasterCharacter,
				//		Start,
				//		HitTarget,
				//		BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime
				//	);
				}
			}
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, fireHit.ImpactPoint, fireHit.ImpactNormal.Rotation());
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,	HitSound, fireHit.ImpactPoint);
		}

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, socketTransform);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* world = GetWorld();
	if (!world)
	{
		return;
	}
	FVector end = TraceStart + (HitTarget - TraceStart) * 1.25f;

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

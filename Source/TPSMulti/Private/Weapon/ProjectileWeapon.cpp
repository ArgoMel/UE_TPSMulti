// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ProjectileWeapon.h"
#include "Weapon/Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	Super::Fire(HitTargets);
	APawn* instigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* muzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLEFLASH);
	UWorld* world = GetWorld();
	if (muzzleFlashSocket && 
		world&&
		ProjectileClass)
	{
		const FTransform socketTransform = muzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		// From muzzle flash socket to hit location from TraceUnderCrosshairs
		const FVector toTarget = HitTargets[0] - socketTransform.GetLocation();
		const FRotator targetRotation = toTarget.Rotation();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetOwner();
		spawnParams.Instigator = instigatorPawn;

		AProjectile* spawnedProjectile;
		if (bUseServerSideRewind)
		{
			if (instigatorPawn->HasAuthority()) // server
			{
				if (instigatorPawn->IsLocallyControlled()) // server, host - use replicated projectile
				{
					spawnedProjectile = world->SpawnActor<AProjectile>(ProjectileClass, socketTransform.GetLocation(), targetRotation, spawnParams);
					spawnedProjectile->bUseServerSideRewind = false;
					spawnedProjectile->Damage = Damage;
					spawnedProjectile->HeadShotDamage = HeadShotDamage;
				}
				else // server, not locally controlled - spawn non-replicated projectile, SSR
				{
					spawnedProjectile = world->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, socketTransform.GetLocation(), targetRotation, spawnParams);
					spawnedProjectile->bUseServerSideRewind = true;
				}
			}
			else // client, using SSR
			{
				if (instigatorPawn->IsLocallyControlled()) // client, locally controlled - spawn non-replicated projectile, use SSR
				{
					spawnedProjectile = world->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, socketTransform.GetLocation(), targetRotation, spawnParams);
					spawnedProjectile->bUseServerSideRewind = true;
					spawnedProjectile->TraceStart = socketTransform.GetLocation();
					spawnedProjectile->InitialVelocity = spawnedProjectile->GetActorForwardVector() * spawnedProjectile->InitialSpeed;
				}
				else // client, not locally controlled - spawn non-replicated projectile, no SSR
				{
					spawnedProjectile = world->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, socketTransform.GetLocation(), targetRotation, spawnParams);
					spawnedProjectile->bUseServerSideRewind = false;
				}
			}
		}
		else // weapon not using SSR
		{
			if (instigatorPawn->HasAuthority())
			{
				spawnedProjectile = world->SpawnActor<AProjectile>(ProjectileClass, socketTransform.GetLocation(), targetRotation, spawnParams);
				spawnedProjectile->bUseServerSideRewind = false;
				spawnedProjectile->Damage = Damage;
				spawnedProjectile->HeadShotDamage = HeadShotDamage;
			}
		}
	}
}

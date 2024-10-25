// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "Character/BaseCharacter.h"
#include <TPSMulti/TPSMulti.h>
//#include "Blaster/PlayerController/BlasterPlayerController.h"
//#include "Blaster/Character/BlasterAnimInstance.h"
//#include "Blaster/Weapon/Projectile.h"
//#include "Blaster/Weapon/Shotgun.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"

constexpr float TraceLength = 80000.f;

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 350.f;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UCombatComponent, CombatState);
	DOREPLIFETIME(UCombatComponent, Grenades);
	DOREPLIFETIME(UCombatComponent, bHoldingTheFlag);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::OnRep_Aiming()
{
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
}

void UCombatComponent::StartFireTimer()
{
}

void UCombatComponent::FireTimerFinished()
{
}

bool UCombatComponent::CanFire()
{
	return true;
}

void UCombatComponent::OnRep_CarriedAmmo()
{
}

void UCombatComponent::InitializeCarriedAmmo()
{
}

void UCombatComponent::OnRep_CombatState()
{
}

void UCombatComponent::UpdateAmmoValues()
{
}

void UCombatComponent::UpdateShotgunAmmoValues()
{
}

void UCombatComponent::OnRep_Grenades()
{
}

void UCombatComponent::UpdateHUDGrenades()
{
}

void UCombatComponent::OnRep_HoldingTheFlag()
{
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (Character)
	{
		if(bIsAiming)
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
		}
		else
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		}
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		if (bIsAiming)
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
		}
		else
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		}
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if(EquippedWeapon&&
		Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		//bCanFire = false;
		//if (EquippedWeapon)
		//{
		//	CrosshairShootingFactor = .75f;

		//	switch (EquippedWeapon->FireType)
		//	{
		//	case EFireType::EFT_Projectile:
				FireProjectileWeapon();
		//		break;
		//	case EFireType::EFT_HitScan:
		//		FireHitScanWeapon();
		//		break;
		//	case EFireType::EFT_Shotgun:
		//		FireShotgun();
		//		break;
		//	}
		//}
		//StartFireTimer();
	}
}

void UCombatComponent::FireProjectileWeapon()
{
	FHitResult hitResult;
	TraceUnderCrosshairs(hitResult);
	ServerFire(hitResult.ImpactPoint, 0.1f);
}

void UCombatComponent::FireHitScanWeapon()
{
}

void UCombatComponent::FireShotgun()
{
}

void UCombatComponent::LocalFire(const FVector_NetQuantize& TraceHitTarget)
{
}

void UCombatComponent::ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	MulticastFire(TraceHitTarget);
}

bool UCombatComponent::ServerFire_Validate(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	return true;
}

void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (!EquippedWeapon)
	{
		return;
	}
	if (Character)
	{
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
}

bool UCombatComponent::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	return false;
}

void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D viewportSize;
	if(GEngine&&
		GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewportSize);
	}
	FVector2D crosshairLoc(viewportSize.X/2.f, viewportSize.Y/2.f);
	FVector crosshairWorldPos;
	FVector crosshairWorldDir;
	bool bScreenToWorld=UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0), crosshairLoc, crosshairWorldPos, crosshairWorldDir);

	if(bScreenToWorld)
	{
		FVector start = crosshairWorldPos;
		FVector end = start + crosshairWorldDir* TraceLength;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, start, end,ECollisionChannel::ECC_Visibility);

		//DrawDebugSphere(GetWorld(),TraceHitResult.ImpactPoint,12.f,12,FColor::Red);
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
}

void UCombatComponent::ServerReload_Implementation()
{
}

void UCombatComponent::HandleReload()
{
}

int32 UCombatComponent::AmountToReload()
{
	return int32();
}

void UCombatComponent::ThrowGrenade()
{
}

void UCombatComponent::ServerThrowGrenade_Implementation()
{
}

void UCombatComponent::DropEquippedWeapon()
{
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
}

void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach)
{
}

void UCombatComponent::AttachFlagToLeftHand(AWeapon* Flag)
{
}

void UCombatComponent::AttachActorToBackpack(AActor* ActorToAttach)
{
}

void UCombatComponent::UpdateCarriedAmmo()
{
}

void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip)
{
}

void UCombatComponent::ReloadEmptyWeapon()
{
}

void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade)
{
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(!Character||
		!WeaponToEquip)
	{
		return;
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* handSocket= Character->GetMesh()->GetSocketByName(SOCKET_RIGHTHAND);
	if(handSocket)
	{
		handSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::SwapWeapons()
{
}

void UCombatComponent::Reload()
{
}

void UCombatComponent::FinishReloading()
{
}

void UCombatComponent::FinishSwap()
{
}

void UCombatComponent::FinishSwapAttachWeapons()
{
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if(bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::ShotgunShellReload()
{
}

void UCombatComponent::JumpToShotgunEnd()
{
}

void UCombatComponent::ThrowGrenadeFinished()
{
}

void UCombatComponent::LaunchGrenade()
{
}

void UCombatComponent::ServerLaunchGrenade_Implementation(const FVector_NetQuantize& Target)
{
}

bool UCombatComponent::ShouldSwapWeapons()
{
	return false;
}

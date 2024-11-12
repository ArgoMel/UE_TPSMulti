// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
//#include "Character/BlasterAnimInstance.h"
//#include "Weapon/Projectile.h"
#include "Weapon/ShotgunWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 350.f;

	for (EWeaponType weaponType : TEnumRange<EWeaponType>())
	{
		CarriedAmmoMap.Emplace(weaponType, 0);
	}
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
		if(UCameraComponent* camera=Character->GetFollowCamera())
		{
			DefaultFOV = camera->FieldOfView;
			CurrentFOV = DefaultFOV;
		}	
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Character && 
		Character->IsLocallyControlled())
	{
		FHitResult hitResult;
		TraceUnderCrosshairs(hitResult);
		HitTarget = hitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

void UCombatComponent::OnRep_Aiming()
{
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if(!EquippedWeapon)
	{
		return;
	}
	if(bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV,EquippedWeapon->GetZoomedFOV(), DeltaTime,EquippedWeapon->GetZoomInterpSpeed() );
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	if(Character&&
		Character->GetFollowCamera())
	{
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCombatComponent::StartFireTimer()
{
	if(!EquippedWeapon)
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(FireTimer,this,&ThisClass::FireTimerFinished, EquippedWeapon->FireDelay);
}

void UCombatComponent::FireTimerFinished()
{
	if (!EquippedWeapon)
	{
		return;
	}
	bCanFire = true;
	if (bFireButtonPressed && 
		EquippedWeapon->bAutomatic)
	{
		Fire();
	}
	ReloadEmptyWeapon();
}

bool UCombatComponent::CanFire()
{
	if (!EquippedWeapon)
	{
		return false;
	}
	if (!EquippedWeapon->IsEmpty() &&
		bCanFire &&
		CombatState == ECombatState::ECS_Reloading &&
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun)
	{
		return true;
	}
	if (bLocallyReloading)
	{
		return false;
	}
	return !EquippedWeapon->IsEmpty() && bCanFire&& CombatState == ECombatState::ECS_Unoccupied;
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	if (!Controller)
	{
		Controller = Cast<ABasePlayerController>(Character->Controller);
	}
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}

	if (CombatState == ECombatState::ECS_Reloading &&
		EquippedWeapon &&
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun &&
		CarriedAmmo == 0)
	{
		JumpToShotgunEnd();
	}
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Unoccupied:
		if(bFireButtonPressed)
		{
			Fire();
		}
		break;
	case ECombatState::ECS_Reloading:
		if (Character && !Character->IsLocallyControlled())
		{
			HandleReload();
		}
		break;
	case ECombatState::ECS_ThrowingGrenade:
		if (Character && !Character->IsLocallyControlled())
		{
			Character->PlayThrowGrenadeMontage();
			AttachActorToLeftHand(EquippedWeapon);
			ShowAttachedGrenade(true);
		}
		break;
	case ECombatState::ECS_SwappingWeapons:
		if (Character && !Character->IsLocallyControlled())
		{
			Character->PlaySwapMontage();
		}
		break;
	}
}

void UCombatComponent::UpdateAmmoValues()
{
	if (!Character ||
		!EquippedWeapon)
	{
		return;
	}

	int32 reloadAmount = AmountToReload();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		if(CarriedAmmoMap[EquippedWeapon->GetWeaponType()]!=INF_AMMO)
		{
			CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= reloadAmount;
		}
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
	if (!Controller)
	{
		Controller = Cast<ABasePlayerController>(Character->Controller);
	}
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
	EquippedWeapon->AddAmmo(reloadAmount);
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
	if(!Character||
		!EquippedWeapon)
	{
		return;
	}
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (bIsAiming)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
	}
	else
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	if(Character->IsLocallyControlled()&&
		EquippedWeapon->GetWeaponType()==EWeaponType::EWT_SniperRifle)
	{
		Character->ShowSniperScopeWidget(bIsAiming);
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
	if(!EquippedWeapon||
		!Character)
	{
		return;
	}
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* handSocket = Character->GetMesh()->GetSocketByName(SOCKET_RIGHTHAND);
	if (handSocket)
	{
		handSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;

	if (EquippedWeapon->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->EquipSound, Character->GetActorLocation());
	}
	EquippedWeapon->SetHUDAmmo();
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		if (EquippedWeapon)
		{
			CrosshairShootingFactor = .75f;

			switch (EquippedWeapon->FireType)
			{
			case EFireType::EFT_Projectile:
				FireProjectileWeapon();
				break;
			case EFireType::EFT_HitScan:
				FireHitScanWeapon();
				break;
			case EFireType::EFT_Shotgun:
				FireShotgun();
				break;
			}
		}
		StartFireTimer();
	}
}

void UCombatComponent::FireProjectileWeapon()
{
	if (EquippedWeapon && Character)
	{
		if (!bAiming)
		{
			HitTarget = EquippedWeapon->TraceEndWithScatter(HitTarget);
		}
		if (!Character->HasAuthority())
		{
			LocalFire(HitTarget);
		}
		ServerFire(HitTarget, EquippedWeapon->FireDelay);
	}
}

void UCombatComponent::FireHitScanWeapon()
{
	if (EquippedWeapon && Character)
	{
		if(!bAiming)
		{
			HitTarget = EquippedWeapon->TraceEndWithScatter(HitTarget);
		}
		if (!Character->HasAuthority())
		{
			LocalFire(HitTarget);
		}
		ServerFire(HitTarget, EquippedWeapon->FireDelay);
	}
}

void UCombatComponent::FireShotgun()
{
	AShotgunWeapon* shotgun = Cast<AShotgunWeapon>(EquippedWeapon);
	if (shotgun && Character)
	{
		TArray<FVector_NetQuantize> hitTargets;
		shotgun->ShotgunTraceEndWithScatter(HitTarget, hitTargets);
		if (!Character->HasAuthority())
		{
			ShotgunLocalFire(hitTargets);
		}
		ServerShotgunFire(hitTargets, EquippedWeapon->FireDelay);
	}
}

void UCombatComponent::LocalFire(const FVector_NetQuantize& TraceHitTarget)
{
	if (!EquippedWeapon)
	{
		return;
	}
	if (Character &&
		CombatState == ECombatState::ECS_Unoccupied)
	{
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	AShotgunWeapon* shotgun = Cast<AShotgunWeapon>(EquippedWeapon);
	if (shotgun == nullptr || Character == nullptr)
	{
		return;
	}
	if (CombatState == ECombatState::ECS_Reloading || 
		CombatState == ECombatState::ECS_Unoccupied)
	{
		bLocallyReloading = false;
		Character->PlayFireMontage(bAiming);
		shotgun->FireShotgun(TraceHitTargets);
		CombatState = ECombatState::ECS_Unoccupied;
	}
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
	if (Character &&
		Character->IsLocallyControlled() && 
		!Character->HasAuthority())
	{
		return;
	}
	LocalFire(TraceHitTarget);
}

void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	MulticastShotgunFire(TraceHitTargets);
}

bool UCombatComponent::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	return true;
}

void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (Character && 
		Character->IsLocallyControlled() && 
		!Character->HasAuthority())
	{
		return;
	}
	ShotgunLocalFire(TraceHitTargets);
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
		if(Character)
		{
			float distToCharacter = (Character->GetActorLocation() - start).Size();
			start += crosshairWorldDir * (distToCharacter+100.f);
			//DrawDebugSphere(GetWorld(), start, 16.f, 12, FColor::Red,false);
		}
		FVector end = start + crosshairWorldDir* TraceLength;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, start, end,ECollisionChannel::ECC_Visibility);

		bFindEnemy = TraceHitResult.GetActor() &&
			TraceHitResult.GetActor()->Implements<UInteractWithCrosshairInterface>();
		if(bFindEnemy)
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}
		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = end;
		}
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if(!Character||
		!Character->Controller)
	{
		return;
	}
	if(!Controller)
	{
		Controller = Cast<ABasePlayerController>(Character->Controller);
	}
	if(Controller)
	{
		if (!HUD)
		{
			HUD = Cast<ABaseHUD>(Controller->GetHUD());
		}
		if(HUD)
		{
			if(EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;				
			}
			else
			{
				HUDPackage.Clear();
			}
			FVector2D walkSpeedRange(0.f,Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D velocityMultiplierRange(0.f,1.f);
			FVector velocity = Character->GetVelocity();

			CrosshairVelocityFactor=FMath::GetMappedRangeValueClamped(walkSpeedRange, velocityMultiplierRange, velocity.Size2D());

			if(Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor,2.25f,DeltaTime,2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			if(bAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor,0.58f,DeltaTime,30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			if (bFindEnemy)
			{
				CrosshairEnemyFactor = FMath::FInterpTo(CrosshairEnemyFactor, 0.1f, DeltaTime, 40.f);
			}
			else
			{
				CrosshairEnemyFactor = FMath::FInterpTo(CrosshairEnemyFactor, 0.f, DeltaTime, 40.f);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor,0.f,DeltaTime,40.f);

			HUDPackage.CrosshairSpread = 0.5f+CrosshairVelocityFactor+ CrosshairInAirFactor-CrosshairAimFactor+CrosshairShootingFactor- CrosshairEnemyFactor;
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::ServerReload_Implementation()
{
	if(!Character||
		!EquippedWeapon)
	{
		return;
	}

	CombatState = ECombatState::ECS_Reloading;
	if (!Character->IsLocallyControlled())
	{
		HandleReload();
	}
}

void UCombatComponent::HandleReload()
{
	if(Character)
	{
		Character->PlayReloadMontage();
	}
}

int32 UCombatComponent::AmountToReload()
{
	if(!EquippedWeapon)
	{
		return 0;
	}
	int32 roomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();

	if(CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 amountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		if(amountCarried==INF_AMMO)
		{
			amountCarried = EquippedWeapon->GetMagCapacity();
		}
		int32 least = FMath::Min(roomInMag,amountCarried);
		return FMath::Clamp(roomInMag,0,least);
	}
	return 0;
}

void UCombatComponent::ThrowGrenade()
{
}

void UCombatComponent::ServerThrowGrenade_Implementation()
{
}

void UCombatComponent::DropEquippedWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
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
	if (!EquippedWeapon)
	{
		return;
	}
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	if (!Controller)
	{
		Controller = Cast<ABasePlayerController>(Character->Controller);
	}
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip)
{
}

void UCombatComponent::ReloadEmptyWeapon()
{
	if (EquippedWeapon && EquippedWeapon->IsEmpty())
	{
		Reload();
	}
}

void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade)
{
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr)
	{
		return;
	}
	DropEquippedWeapon();
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* handSocket = Character->GetMesh()->GetSocketByName(SOCKET_RIGHTHAND);
	if (handSocket)
	{
		handSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	if (EquippedWeapon->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->EquipSound, Character->GetActorLocation());
	}
	ReloadEmptyWeapon();
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(!Character||
		!WeaponToEquip||
		CombatState != ECombatState::ECS_Unoccupied)
	{
		return;
	}
	EquipPrimaryWeapon(WeaponToEquip);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::SwapWeapons()
{
}

void UCombatComponent::Reload()
{
	if ((CarriedAmmo > 0|| CarriedAmmo==INF_AMMO) &&
		CombatState == ECombatState::ECS_Unoccupied && 
		EquippedWeapon && 
		!EquippedWeapon->IsFull() &&
		!bLocallyReloading)
	{
		ServerReload();
		HandleReload();
		bLocallyReloading = true;
	}
}

void UCombatComponent::FinishReloading()
{
	if(!Character)
	{
		return;
	}
	bLocallyReloading = false;
	if(Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}	
	if(bFireButtonPressed)
	{
		Fire();
	}
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

void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
}

bool UCombatComponent::ShouldSwapWeapons()
{
	return false;
}

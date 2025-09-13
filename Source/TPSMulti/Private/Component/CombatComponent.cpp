// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Weapon/Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"

UCombatComponent::UCombatComponent()
: Character(nullptr)
, Controller(nullptr)
, HUD(nullptr)
, EquippedWeapon(nullptr)
, SecondaryWeapon(nullptr)
, bFireButtonPressed(false)
, CrosshairVelocityFactor(0)
, CrosshairInAirFactor(0)
, CrosshairAimFactor(0)
, CrosshairShootingFactor(0)
, CrosshairEnemyFactor(0)
, HitTarget()
, DefaultFOV(0)
, CurrentFOV(0)
, CarriedAmmo(0)
, TheFlag(nullptr)
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
		if(const UCameraComponent* camera=Character->GetFollowCamera())
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
	if (Character&&Character->IsLocallyControlled())
	{
		bAiming=bAimButtonPressed;
	}
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

bool UCombatComponent::CanFire() const
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
	default: ;
	}
}

void UCombatComponent::UpdateAmmoValues()
{
	if (!Character ||
		!EquippedWeapon)
	{
		return;
	}

	const int32 reloadAmount = AmountToReload();
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
	if (!Character ||
		!EquippedWeapon)
	{
		return;
	}

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		if (CarriedAmmoMap[EquippedWeapon->GetWeaponType()] != INF_AMMO)
		{
			CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= 1;
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
	EquippedWeapon->AddAmmo(1);
	bCanFire = true;
	if(EquippedWeapon->IsFull()||
		CarriedAmmo==0)
	{
		JumpToShotgunEnd();
	}
}

void UCombatComponent::OnRep_Grenades()
{
	UpdateHUDGrenades();
}

void UCombatComponent::UpdateHUDGrenades()
{
	if(!Controller)
	{
		Controller =Cast<ABasePlayerController>(Character->Controller);
	}	
	if (Controller)
	{
		Controller->SetHUDGrenades(Grenades);
	}
}

void UCombatComponent::OnRep_HoldingTheFlag() const
{
	if (bHoldingTheFlag&&
		Character&&
		Character->IsLocallyControlled())
	{
		Character->Crouch();
	}
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
	if(Character->IsLocallyControlled())
	{
		if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
		{
			Character->ShowSniperScopeWidget(bIsAiming);
		}
		bAimButtonPressed = bIsAiming;
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

void UCombatComponent::OnRep_EquippedWeapon() const
{
	if(!EquippedWeapon||
		!Character)
	{
		return;
	}
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorToRightHand(EquippedWeapon);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
	PlayEquipWeaponSound(EquippedWeapon);
	EquippedWeapon->EnableCustomDepth(false);
	EquippedWeapon->SetHUDAmmo();
}

void UCombatComponent::OnRep_SecondaryWeapon() const
{
	if (SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
		AttachActorToBackpack(SecondaryWeapon);
		PlayEquipWeaponSound(EquippedWeapon);
	}
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		if (EquippedWeapon)
		{
			CrosshairShootingFactor = .75f;
			FireWeapon();
		}
		StartFireTimer();
	}
}

void UCombatComponent::FireWeapon()
{
	if (EquippedWeapon && Character)
	{
		TArray<FVector_NetQuantize> hitTargets;
		if (!bAiming)
		{
			EquippedWeapon->TraceEndWithScatter(HitTarget, hitTargets);
		}
		else
		{
			hitTargets.Emplace(HitTarget);
		}
		if (!Character->HasAuthority())
		{
			LocalFire(hitTargets);
		}
		ServerFire(hitTargets, EquippedWeapon->FireDelay);
	}
}

void UCombatComponent::LocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (!EquippedWeapon||
		!Character)
	{
		return;
	}
	if(EquippedWeapon->GetWeaponType()==EWeaponType::EWT_Shotgun)
	{
		if (CombatState == ECombatState::ECS_Reloading ||
			CombatState == ECombatState::ECS_Unoccupied)
		{
			bLocallyReloading = false;
			Character->PlayFireMontage(bAiming);
			EquippedWeapon->Fire(TraceHitTargets);
			CombatState = ECombatState::ECS_Unoccupied;
		}
	}
	else
	{
		if (CombatState == ECombatState::ECS_Unoccupied)
		{
			Character->PlayFireMontage(bAiming);
			EquippedWeapon->Fire(TraceHitTargets);
		}
	}

}

void UCombatComponent::ServerFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	MulticastFire(TraceHitTargets);
}

bool UCombatComponent::ServerFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	if (EquippedWeapon)
	{
		return FMath::IsNearlyEqual(EquippedWeapon->FireDelay,FireDelay,0.001f);
	}
	return true;
}

void UCombatComponent::MulticastFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (Character &&
		Character->IsLocallyControlled() && 
		!Character->HasAuthority())
	{
		return;
	}
	LocalFire(TraceHitTargets);
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D viewportSize=FVector2D::ZeroVector;
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

void UCombatComponent::HandleReload() const
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
	const int32 roomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();

	if(CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 amountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		if(amountCarried==INF_AMMO)
		{
			amountCarried = EquippedWeapon->GetMagCapacity();
		}
		const int32 least = FMath::Min(roomInMag,amountCarried);
		return FMath::Clamp(roomInMag,0,least);
	}
	return 0;
}

void UCombatComponent::ThrowGrenade()
{
	if (!Character||
		CombatState != ECombatState::ECS_Unoccupied||
		!EquippedWeapon||
		Grenades==0)
	{
		return;
	}
	CombatState = ECombatState::ECS_ThrowingGrenade;
	Character->PlayThrowGrenadeMontage();
	AttachActorToLeftHand(EquippedWeapon);
	ShowAttachedGrenade(true);
	if (Character->HasAuthority())
	{
		if(Grenades!=INF_AMMO)
		{
			Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
		}
		UpdateHUDGrenades();
	}
	else
	{
		ServerThrowGrenade();
	}
}

void UCombatComponent::ServerThrowGrenade_Implementation()
{
	if (Grenades == 0)
	{
		return;
	}
	CombatState = ECombatState::ECS_ThrowingGrenade;
	if (Character)
	{
		Character->PlayThrowGrenadeMontage();
		AttachActorToLeftHand(EquippedWeapon);
		ShowAttachedGrenade(true);
	}
	if (Grenades != INF_AMMO)
	{
		Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
	}
	UpdateHUDGrenades();
}

void UCombatComponent::DropEquippedWeapon() const
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach) const
{
	if(!Character||
		!Character->GetMesh()||
		!ActorToAttach)
	{
		return;
	}
	const USkeletalMeshSocket* handSocket = Character->GetMesh()->GetSocketByName(SOCKET_RIGHTHAND);
	if (handSocket)
	{
		handSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach) const
{
	if (!Character ||
		!Character->GetMesh() ||
		!ActorToAttach||
		!EquippedWeapon)
	{
		return;
	}
	FName socketName;
	if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol || 
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SubmachineGun)
	{
		socketName = SOCKET_PISTOL;
	}
	else
	{
		socketName = SOCKET_LEFTHAND;
	}

	const USkeletalMeshSocket* handSocket = Character->GetMesh()->GetSocketByName(socketName);
	if (handSocket)
	{
		handSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::AttachFlagToLeftHand(AWeapon* Flag) const
{
	if (!Character ||
	!Character->GetMesh() ||
	!Flag||
	!EquippedWeapon)
	{
		return;
	}
	
	const USkeletalMeshSocket* handSocket = Character->GetMesh()->GetSocketByName(SOCKET_FLAG);
	if (handSocket)
	{
		handSocket->AttachActor(Flag, Character->GetMesh());
	}
}

void UCombatComponent::AttachActorToBackpack(AActor* ActorToAttach) const
{
	if (!Character ||
		!Character->GetMesh() ||
		!ActorToAttach)
	{
		return;
	}
	const USkeletalMeshSocket* backpackSocket = Character->GetMesh()->GetSocketByName(SOCKET_BACKPACK);
	if (backpackSocket)
	{
		backpackSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
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

void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip) const
{
	if (Character&& WeaponToEquip&&WeaponToEquip->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponToEquip->EquipSound, Character->GetActorLocation());
	}
}

void UCombatComponent::ReloadEmptyWeapon()
{
	if (EquippedWeapon && EquippedWeapon->IsEmpty())
	{
		Reload();
	}
}

void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade) const
{
	if(Character&&Character->GetAttachedGrenade())
	{
		Character->GetAttachedGrenade()->SetVisibility(bShowGrenade);
	}
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
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	PlayEquipWeaponSound(EquippedWeapon);
	ReloadEmptyWeapon();
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip)
	{
		return;
	}
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackpack(WeaponToEquip);
	PlayEquipWeaponSound(WeaponToEquip);
	SecondaryWeapon->SetOwner(Character);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(!Character||
		!WeaponToEquip||
		CombatState != ECombatState::ECS_Unoccupied)
	{
		return;
	}
	if(WeaponToEquip->GetWeaponType() == EWeaponType::EWT_Flag)
	{
		Character->Crouch();
		bHoldingTheFlag = true;
		WeaponToEquip->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachFlagToLeftHand(WeaponToEquip);
		WeaponToEquip->SetOwner(Character);
		TheFlag = WeaponToEquip;
	}
	else
	{
		if (EquippedWeapon && !SecondaryWeapon)
		{
			EquipSecondaryWeapon(WeaponToEquip);
		}
		else
		{
			EquipPrimaryWeapon(WeaponToEquip);
		}
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::SwapWeapons()
{
	if(CombatState!=ECombatState::ECS_Unoccupied||
		!Character||
		!Character->HasAuthority())
	{
		return;
	}
	
	Character->PlaySwapMontage();
	Character->bFinishedSwapping=false;
	CombatState = ECombatState::ECS_SwappingWeapons;
	if (SecondaryWeapon)
	{
		SecondaryWeapon->EnableCustomDepth(false);
	}
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
	if (!Character)
	{
		return;
	}
	if(Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
	}
	Character->bFinishedSwapping=true;
	if (SecondaryWeapon)
	{
		SecondaryWeapon->EnableCustomDepth(true);
	}
}

void UCombatComponent::FinishSwapAttachWeapons()
{
	PlayEquipWeaponSound(SecondaryWeapon);

	if (!Character  ||
		!Character->HasAuthority())
	{
		return;
	}
	AWeapon* tempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = tempWeapon;
	
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackpack(SecondaryWeapon);
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
	if(Character&&
		Character->HasAuthority())
	{
		UpdateShotgunAmmoValues();
	}	
}

void UCombatComponent::JumpToShotgunEnd() const
{
	UAnimInstance* animInstance = Character->GetMesh()->GetAnimInstance();
	if (animInstance && Character->GetReloadMontage())
	{
		animInstance->Montage_JumpToSection(SECTION_SHOTGUNEND);
	}
}

void UCombatComponent::ThrowGrenadeFinished()
{
	CombatState = ECombatState::ECS_Unoccupied;
	AttachActorToRightHand(EquippedWeapon);
}

void UCombatComponent::LaunchGrenade()
{
	ShowAttachedGrenade(false);
	if (Character && Character->IsLocallyControlled())
	{
		ServerLaunchGrenade(HitTarget);
	}
}

void UCombatComponent::ServerLaunchGrenade_Implementation(const FVector_NetQuantize& Target)
{
	if (Character &&
		GrenadeClass &&
		Character->GetAttachedGrenade())
	{
		const FVector startingLoc = Character->GetAttachedGrenade()->GetComponentLocation();
		const FVector toTarget = HitTarget - startingLoc;
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Character;
		spawnParams.Instigator = Character;
		UWorld* world = GetWorld();
		if (world)
		{
			world->SpawnActor<AProjectile>(GrenadeClass, startingLoc, toTarget.Rotation(), spawnParams);
		}
	}
}

void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	if(CarriedAmmoMap.Contains(WeaponType)&&
		CarriedAmmoMap[WeaponType]!=INF_AMMO)
	{
		CarriedAmmoMap[WeaponType] =FMath::Clamp(CarriedAmmoMap[WeaponType]+ AmmoAmount,0,MaxCarriedAmmo);
		UpdateCarriedAmmo();
	}
	if(EquippedWeapon&&
		EquippedWeapon->IsEmpty()&&
		EquippedWeapon->GetWeaponType()==WeaponType)
	{
		Reload();
	}
}

bool UCombatComponent::ShouldSwapWeapons() const
{
	return EquippedWeapon&&SecondaryWeapon;
}

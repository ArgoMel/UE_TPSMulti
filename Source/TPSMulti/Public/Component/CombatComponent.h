// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <TPSMulti/TPSMulti.h>
#include "Components/ActorComponent.h"
#include "HUD/BaseHUD.h"
#include "CombatComponent.generated.h"

class ABaseCharacter;
class ABasePlayerController;
class ABaseHUD;
class AWeapon;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSMULTI_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ABaseCharacter;
public:	
	UCombatComponent();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ABaseCharacter* Character;
	UPROPERTY()
	ABasePlayerController* Controller;
	UPROPERTY()
	ABaseHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;

	bool bAimButtonPressed = false;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;
	
	// HUD and crosshairs
	bool bFindEnemy = false;
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	float CrosshairEnemyFactor;

	FVector HitTarget;

	FHUDPackage HUDPackage;

	// Aiming and FOV
	// Field of view when not aiming; set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	// Automatic fire
	FTimerHandle FireTimer;
	bool bCanFire = true;

	// Carried ammo for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UPROPERTY(EditDefaultsOnly,ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 4;

	UPROPERTY(EditAnywhere)
	int32 MaxGrenades = 4;

	UPROPERTY(ReplicatedUsing = OnRep_HoldingTheFlag)
	bool bHoldingTheFlag = false;

	UPROPERTY()
	AWeapon* TheFlag;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> GrenadeClass;

public:
	bool bLocallyReloading = false;

private:
	UFUNCTION()
	void OnRep_Aiming();

	void InterpFOV(float DeltaTime);

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire() const;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();
	void UpdateShotgunAmmoValues();

	UFUNCTION()
	void OnRep_Grenades();

	void UpdateHUDGrenades();

	UFUNCTION()
	void OnRep_HoldingTheFlag();

protected:
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);
	void ServerSetAiming_Implementation(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon() const;

	UFUNCTION()
	void OnRep_SecondaryWeapon() const;

	void Fire();
	void FireWeapon();
	void LocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);
	void ServerFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);
	bool ServerFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const TArray<FVector_NetQuantize>& TraceHitTargets);
	void MulticastFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();
	void ServerReload_Implementation();

	void HandleReload() const;
	int32 AmountToReload();

	void ThrowGrenade();

	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();
	void ServerThrowGrenade_Implementation();

	void DropEquippedWeapon() const;
	void AttachActorToRightHand(AActor* ActorToAttach) const;
	void AttachActorToLeftHand(AActor* ActorToAttach) const;
	void AttachFlagToLeftHand(AWeapon* Flag);
	void AttachActorToBackpack(AActor* ActorToAttach) const;
	void UpdateCarriedAmmo();
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip) const;
	void ReloadEmptyWeapon();
	void ShowAttachedGrenade(bool bShowGrenade) const;
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

public:
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void SwapWeapons();
	void Reload();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void JumpToShotgunEnd() const;

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);
	void ServerLaunchGrenade_Implementation(const FVector_NetQuantize& Target);

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	bool ShouldSwapWeapons() const;

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE int32 GetCarriedAmmo() const { return CarriedAmmo; }
	FORCEINLINE int32 GetGrenades() const { return Grenades; }
};

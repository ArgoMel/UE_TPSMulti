// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
//#include "Blaster/Weapon/Weapon.h"
//#include "Blaster/BlasterComponents/CombatComponent.h"
//#include "Blaster/BlasterComponents/BuffComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
//#include "BlasterAnimInstance.h"
//#include "Blaster/Blaster.h"
//#include "Blaster/PlayerController/BlasterPlayerController.h"
//#include "Blaster/GameMode/BlasterGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
//#include "Blaster/PlayerState/BlasterPlayerState.h"
//#include "Blaster/Weapon/WeaponTypes.h"
#include "Components/BoxComponent.h"
//#include "Blaster/BlasterComponents/LagCompensationComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
//#include "Blaster/GameState/BlasterGameState.h"
//#include "Blaster/PlayerStart/TeamPlayerStart.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Jump()
{
	Super::Jump();
}

void ABaseCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
}

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();
}

void ABaseCharacter::ServerEquipButtonPressed_Implementation()
{
}

void ABaseCharacter::TurnInPlace(float DeltaTime)
{
}

void ABaseCharacter::HideCameraIfCharacterClose()
{
}

float ABaseCharacter::CalculateSpeed()
{
	return 0.0f;
}

void ABaseCharacter::OnRep_Health(float LastHealth)
{
}

void ABaseCharacter::OnRep_Shield(float LastShield)
{
}

void ABaseCharacter::ElimTimerFinished()
{
}

void ABaseCharacter::UpdateDissolveMaterial(float DissolveValue)
{
}

void ABaseCharacter::StartDissolve()
{
}

void ABaseCharacter::MoveForward(float Value)
{
}

void ABaseCharacter::MoveRight(float Value)
{
}

void ABaseCharacter::Turn(float Value)
{
}

void ABaseCharacter::LookUp(float Value)
{
}

void ABaseCharacter::EquipButtonPressed()
{
}

void ABaseCharacter::CrouchButtonPressed()
{
}

void ABaseCharacter::ReloadButtonPressed()
{
}

void ABaseCharacter::AimButtonPressed()
{
}

void ABaseCharacter::AimButtonReleased()
{
}

void ABaseCharacter::AimOffset(float DeltaTime)
{
}

void ABaseCharacter::CalculateAO_Pitch()
{
}

void ABaseCharacter::SimProxiesTurn()
{
}

void ABaseCharacter::FireButtonPressed()
{
}

void ABaseCharacter::FireButtonReleased()
{
}

void ABaseCharacter::PlayHitReactMontage()
{
}

void ABaseCharacter::GrenadeButtonPressed()
{
}

void ABaseCharacter::DropOrDestroyWeapons()
{
}

void ABaseCharacter::SetSpawnPoint()
{
}

void ABaseCharacter::OnPlayerStateInitialized()
{
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
}

void ABaseCharacter::PollInit()
{
}

void ABaseCharacter::RotateInPlace(float DeltaTime)
{
}

void ABaseCharacter::PlayFireMontage(bool bAiming)
{
}

void ABaseCharacter::PlayReloadMontage()
{
}

void ABaseCharacter::PlayElimMontage()
{
}

void ABaseCharacter::PlayThrowGrenadeMontage()
{
}

void ABaseCharacter::PlaySwapMontage()
{
}

void ABaseCharacter::Elim(bool bPlayerLeftGame)
{
}

void ABaseCharacter::MulticastElim_Implementation(bool bPlayerLeftGame)
{
}

void ABaseCharacter::UpdateHUDHealth()
{
}

void ABaseCharacter::UpdateHUDShield()
{
}

void ABaseCharacter::UpdateHUDAmmo()
{
}

void ABaseCharacter::SpawDefaultWeapon()
{
}

void ABaseCharacter::ServerLeaveGame_Implementation()
{
}

void ABaseCharacter::MulticastGainedTheLead_Implementation()
{
}

void ABaseCharacter::MulticastLostTheLead_Implementation()
{
}

bool ABaseCharacter::IsWeaponEquipped()
{
	return false;
}

bool ABaseCharacter::IsAiming()
{
	return false;
}

FVector ABaseCharacter::GetHitTarget() const
{
	return FVector();
}

bool ABaseCharacter::IsLocallyReloading()
{
	return false;
}

bool ABaseCharacter::IsHoldingTheFlag() const
{
	return false;
}

void ABaseCharacter::SetHoldingTheFlag(bool bHolding)
{
}

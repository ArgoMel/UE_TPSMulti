// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "Weapon/Weapon.h"
#include "Component/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (BaseCharacter == nullptr)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}
	if (BaseCharacter == nullptr) 
	{
		return;
	}

	const FVector velocity = BaseCharacter->GetVelocity();
	Speed = velocity.Size2D();
	bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size()>0.f;
	bWeaponEquipped = BaseCharacter->IsWeaponEquipped();
	EquippedWeapon = BaseCharacter->GetEquippedWeapon();
	bIsCrouched = BaseCharacter->bIsCrouched;
	bAiming = BaseCharacter->IsAiming();
	TurningInPlace = BaseCharacter->GetTurningInPlace();
	bRotateRootBone = BaseCharacter->ShouldRotateRootBone();
	bElimmed = BaseCharacter->IsElimmed();
	bHoldingTheFlag = BaseCharacter->IsHoldingTheFlag();

	// Offset Yaw for Strafing
	const FRotator aimRotation = BaseCharacter->GetBaseAimRotation();
	const FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
	const FRotator deltaRot = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation, aimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, deltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BaseCharacter->GetActorRotation();
	const FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float target = delta.Yaw / DeltaTime;
	const float interp = FMath::FInterpTo(Lean, target, DeltaTime, 6.f);
	Lean = FMath::Clamp(interp, -90.f, 90.f);

	AO_Yaw = BaseCharacter->GetAO_Yaw();
	AO_Pitch = BaseCharacter->GetAO_Pitch();

	if (bWeaponEquipped && 
		EquippedWeapon && 
		EquippedWeapon->GetWeaponMesh() && 
		BaseCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(SOCKET_LEFTHAND, ERelativeTransformSpace::RTS_World);
		FVector outPosition;
		FRotator outRotation;
		BaseCharacter->GetMesh()->TransformToBoneSpace(BONE_RIGHTHAND, LeftHandTransform.GetLocation(), FRotator::ZeroRotator, outPosition, outRotation);
		LeftHandTransform.SetLocation(outPosition);
		LeftHandTransform.SetRotation(FQuat(outRotation));

		if (BaseCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			const FTransform rightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(BONE_RIGHTHAND, ERelativeTransformSpace::RTS_World);
			const FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(rightHandTransform.GetLocation(), rightHandTransform.GetLocation() + (rightHandTransform.GetLocation() - BaseCharacter->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, lookAtRotation, DeltaTime, 30.f);
		}
	}

	bUseFABRIK = BaseCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	if (BaseCharacter->IsLocallyControlled() &&
		BaseCharacter->GetCombatState() != ECombatState::ECS_ThrowingGrenade &&
		BaseCharacter->bFinishedSwapping)
	{
		bUseFABRIK = !BaseCharacter->IsLocallyReloading();
	}
	bUseAimOffsets = BaseCharacter->GetCombatState() == ECombatState::ECS_Unoccupied &&
					!BaseCharacter->GetDisableGameplay();
	bTransformRightHand = BaseCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && 
						!BaseCharacter->GetDisableGameplay();
}

void UBaseAnimInstance::AnimNotify_ReloadFinished() const
{
	if(IsValid(BaseCharacter)&&
		IsValid(BaseCharacter->GetCombat()))
	{
		BaseCharacter->GetCombat()->FinishReloading();
	}
}

void UBaseAnimInstance::AnimNotify_Shell() const
{
	if (IsValid(BaseCharacter) &&
		IsValid(BaseCharacter->GetCombat()))
	{
		BaseCharacter->GetCombat()->ShotgunShellReload();
	}
}

void UBaseAnimInstance::AnimNotify_FinishGrenadeThrow() const
{
	if (IsValid(BaseCharacter) &&
		IsValid(BaseCharacter->GetCombat()))
	{
		BaseCharacter->GetCombat()->ThrowGrenadeFinished();
	}
}

void UBaseAnimInstance::AnimNotify_GrenadeLaunch() const
{
	if (IsValid(BaseCharacter) &&
		IsValid(BaseCharacter->GetCombat()))
	{
		BaseCharacter->GetCombat()->LaunchGrenade();
	}
}

void UBaseAnimInstance::AnimNotify_SwapAttachWeapons() const
{
	if (IsValid(BaseCharacter) &&
	IsValid(BaseCharacter->GetCombat()))
	{
		BaseCharacter->GetCombat()->FinishSwapAttachWeapons();
	}
}

void UBaseAnimInstance::AnimNotify_SwapFinished() const
{
	if (IsValid(BaseCharacter) &&
	IsValid(BaseCharacter->GetCombat()))
	{
		BaseCharacter->GetCombat()->FinishSwap();
	}
}

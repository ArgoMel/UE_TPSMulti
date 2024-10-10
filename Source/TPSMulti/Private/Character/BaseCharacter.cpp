// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
//#include "Blaster/Weapon/Weapon.h"
//#include "Blaster/BlasterComponents/CombatComponent.h"
//#include "Blaster/BlasterComponents/BuffComponent.h"
//#include "BlasterAnimInstance.h"
//#include "Blaster/Blaster.h"
//#include "Blaster/PlayerController/BlasterPlayerController.h"
//#include "Blaster/GameMode/BlasterGameMode.h"
//#include "Blaster/PlayerState/BlasterPlayerState.h"
//#include "Blaster/Weapon/WeaponTypes.h"
//#include "Blaster/BlasterComponents/LagCompensationComponent.h"
//#include "Blaster/GameState/BlasterGameState.h"
//#include "Blaster/PlayerStart/TeamPlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	//Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	//Combat->SetIsReplicated(true);
	//
	//Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	//Buff->SetIsReplicated(true);

	//LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensation"));

	//TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));

	AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attached Grenade"));
	AttachedGrenade->SetupAttachment(GetMesh(), FName("GrenadeSocket"));
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Hit boxes for server-side rewind
	head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	head->SetupAttachment(GetMesh(), FName("head"));
	HitCollisionBoxes.Add(FName("head"), head);

	pelvis = CreateDefaultSubobject<UBoxComponent>(TEXT("pelvis"));
	pelvis->SetupAttachment(GetMesh(), FName("pelvis"));
	HitCollisionBoxes.Add(FName("pelvis"), pelvis);

	spine_02 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_02"));
	spine_02->SetupAttachment(GetMesh(), FName("spine_02"));
	HitCollisionBoxes.Add(FName("spine_02"), spine_02);

	spine_03 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_03"));
	spine_03->SetupAttachment(GetMesh(), FName("spine_03"));
	HitCollisionBoxes.Add(FName("spine_03"), spine_03);

	upperarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_l"));
	upperarm_l->SetupAttachment(GetMesh(), FName("upperarm_l"));
	HitCollisionBoxes.Add(FName("upperarm_l"), upperarm_l);

	upperarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_r"));
	upperarm_r->SetupAttachment(GetMesh(), FName("upperarm_r"));
	HitCollisionBoxes.Add(FName("upperarm_r"), upperarm_r);

	lowerarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_l"));
	lowerarm_l->SetupAttachment(GetMesh(), FName("lowerarm_l"));
	HitCollisionBoxes.Add(FName("lowerarm_l"), lowerarm_l);

	lowerarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_r"));
	lowerarm_r->SetupAttachment(GetMesh(), FName("lowerarm_r"));
	HitCollisionBoxes.Add(FName("lowerarm_r"), lowerarm_r);

	hand_l = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_l"));
	hand_l->SetupAttachment(GetMesh(), FName("hand_l"));
	HitCollisionBoxes.Add(FName("hand_l"), hand_l);

	hand_r = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_r"));
	hand_r->SetupAttachment(GetMesh(), FName("hand_r"));
	HitCollisionBoxes.Add(FName("hand_r"), hand_r);

	blanket = CreateDefaultSubobject<UBoxComponent>(TEXT("blanket"));
	blanket->SetupAttachment(GetMesh(), FName("backpack"));
	HitCollisionBoxes.Add(FName("blanket"), blanket);

	backpack = CreateDefaultSubobject<UBoxComponent>(TEXT("backpack"));
	backpack->SetupAttachment(GetMesh(), FName("backpack"));
	HitCollisionBoxes.Add(FName("backpack"), backpack);

	thigh_l = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_l"));
	thigh_l->SetupAttachment(GetMesh(), FName("thigh_l"));
	HitCollisionBoxes.Add(FName("thigh_l"), thigh_l);

	thigh_r = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_r"));
	thigh_r->SetupAttachment(GetMesh(), FName("thigh_r"));
	HitCollisionBoxes.Add(FName("thigh_r"), thigh_r);

	calf_l = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_l"));
	calf_l->SetupAttachment(GetMesh(), FName("calf_l"));
	HitCollisionBoxes.Add(FName("calf_l"), calf_l);

	calf_r = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_r"));
	calf_r->SetupAttachment(GetMesh(), FName("calf_r"));
	HitCollisionBoxes.Add(FName("calf_r"), calf_r);

	foot_l = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_l"));
	foot_l->SetupAttachment(GetMesh(), FName("foot_l"));
	HitCollisionBoxes.Add(FName("foot_l"), foot_l);

	foot_r = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_r"));
	foot_r->SetupAttachment(GetMesh(), FName("foot_r"));
	HitCollisionBoxes.Add(FName("foot_r"), foot_r);

	for (auto& box : HitCollisionBoxes)
	{
		if (box.Value)
		{
			//box.Value->SetCollisionObjectType(ECC_HitBox);
			box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//box.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
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

void ABaseCharacter::Move(FVector2D Value)
{
	if (!Controller||
		Value.IsZero())
	{
		return;
	}
	const FRotator yawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector dirX = FRotationMatrix(yawRot).GetUnitAxis(EAxis::X);
	const FVector dirY = FRotationMatrix(yawRot).GetUnitAxis(EAxis::Y);
	AddMovementInput(dirX, Value.X);
	AddMovementInput(dirY, Value.Y);
}

void ABaseCharacter::LookAround(FVector2D Value)
{
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(-Value.Y);
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

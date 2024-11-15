// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
#include "Weapon/Weapon.h"
#include "Component/CombatComponent.h"
#include "Component/BuffComponent.h"
#include "Character/BaseAnimInstance.h"
#include "PlayerController/BasePlayerController.h"
#include "GameMode/BaseGameMode.h"
#include "PlayerState/BasePlayerState.h"
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
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AO_Yaw = 0.f;
	InterpAO_Yaw = 0.f;
	AO_Pitch = 0.f;
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
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

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);
	
	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	Buff->SetIsReplicated(true);

	//LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensation"));

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));

	AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attached Grenade"));
	AttachedGrenade->SetupAttachment(GetMesh(), SOCKET_GRENADE);
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Hit boxes for server-side rewind
	head = CreateDefaultSubobject<UBoxComponent>(BONE_HEAD);
	head->SetupAttachment(GetMesh(), BONE_HEAD);
	HitCollisionBoxes.Add(BONE_HEAD, head);

	pelvis = CreateDefaultSubobject<UBoxComponent>(BONE_PELVIS);
	pelvis->SetupAttachment(GetMesh(), BONE_PELVIS);
	HitCollisionBoxes.Add(BONE_PELVIS, pelvis);

	spine_02 = CreateDefaultSubobject<UBoxComponent>(BONE_PELVIS2);
	spine_02->SetupAttachment(GetMesh(), BONE_PELVIS2);
	HitCollisionBoxes.Add(BONE_PELVIS2, spine_02);

	spine_03 = CreateDefaultSubobject<UBoxComponent>(BONE_PELVIS3);
	spine_03->SetupAttachment(GetMesh(), BONE_PELVIS3);
	HitCollisionBoxes.Add(BONE_PELVIS3, spine_03);

	upperarm_l = CreateDefaultSubobject<UBoxComponent>(BONE_LEFTUPPERARM);
	upperarm_l->SetupAttachment(GetMesh(), BONE_LEFTUPPERARM);
	HitCollisionBoxes.Add(BONE_LEFTUPPERARM, upperarm_l);

	upperarm_r = CreateDefaultSubobject<UBoxComponent>(BONE_RIGHTUPPERARM);
	upperarm_r->SetupAttachment(GetMesh(), BONE_RIGHTUPPERARM);
	HitCollisionBoxes.Add(BONE_RIGHTUPPERARM, upperarm_r);

	lowerarm_l = CreateDefaultSubobject<UBoxComponent>(BONE_LEFTLOWERARM);
	lowerarm_l->SetupAttachment(GetMesh(), BONE_LEFTLOWERARM);
	HitCollisionBoxes.Add(BONE_LEFTLOWERARM, lowerarm_l);

	lowerarm_r = CreateDefaultSubobject<UBoxComponent>(BONE_RIGHTLOWERARM);
	lowerarm_r->SetupAttachment(GetMesh(), BONE_RIGHTLOWERARM);
	HitCollisionBoxes.Add(BONE_RIGHTLOWERARM, lowerarm_r);

	hand_l = CreateDefaultSubobject<UBoxComponent>(BONE_LEFTHAND);
	hand_l->SetupAttachment(GetMesh(), BONE_LEFTHAND);
	HitCollisionBoxes.Add(BONE_LEFTHAND, hand_l);

	hand_r = CreateDefaultSubobject<UBoxComponent>(BONE_RIGHTHAND);
	hand_r->SetupAttachment(GetMesh(), BONE_RIGHTHAND);
	HitCollisionBoxes.Add(BONE_RIGHTHAND, hand_r);

	blanket = CreateDefaultSubobject<UBoxComponent>(BONE_BLANKET);
	blanket->SetupAttachment(GetMesh(), BONE_BLANKET);
	HitCollisionBoxes.Add(BONE_BLANKET, blanket);

	backpack = CreateDefaultSubobject<UBoxComponent>(BONE_BACKPACK);
	backpack->SetupAttachment(GetMesh(), BONE_BACKPACK);
	HitCollisionBoxes.Add(BONE_BACKPACK, backpack);

	thigh_l = CreateDefaultSubobject<UBoxComponent>(BONE_LEFTTHIGH);
	thigh_l->SetupAttachment(GetMesh(), BONE_LEFTTHIGH);
	HitCollisionBoxes.Add(BONE_LEFTTHIGH, thigh_l);

	thigh_r = CreateDefaultSubobject<UBoxComponent>(BONE_RIGHTTHIGH);
	thigh_r->SetupAttachment(GetMesh(), BONE_RIGHTTHIGH);
	HitCollisionBoxes.Add(BONE_RIGHTTHIGH, thigh_r);

	calf_l = CreateDefaultSubobject<UBoxComponent>(BONE_LEFTCALF);
	calf_l->SetupAttachment(GetMesh(), BONE_LEFTCALF);
	HitCollisionBoxes.Add(BONE_LEFTCALF, calf_l);

	calf_r = CreateDefaultSubobject<UBoxComponent>(BONE_RIGHTCALF);
	calf_r->SetupAttachment(GetMesh(), BONE_RIGHTCALF);
	HitCollisionBoxes.Add(BONE_RIGHTCALF, calf_r);

	foot_l = CreateDefaultSubobject<UBoxComponent>(BONE_LEFTFOOT);
	foot_l->SetupAttachment(GetMesh(), BONE_LEFTFOOT);
	HitCollisionBoxes.Add(BONE_LEFTFOOT, foot_l);

	foot_r = CreateDefaultSubobject<UBoxComponent>(BONE_RIGHTFOOT);
	foot_r->SetupAttachment(GetMesh(), BONE_RIGHTFOOT);
	HitCollisionBoxes.Add(BONE_RIGHTFOOT, foot_r);

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
	DOREPLIFETIME_CONDITION(ThisClass, OverlappingWeapon,COND_OwnerOnly);
	DOREPLIFETIME(ThisClass, Health);
	DOREPLIFETIME(ThisClass, Shield);
	DOREPLIFETIME(ThisClass, bDisableGameplay);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->Character = this;
	}
	if (Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeeds(GetCharacterMovement()->MaxWalkSpeed,GetCharacterMovement()->MaxWalkSpeedCrouched);
		Buff->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}
	//if (LagCompensation)
	//{
	//	LagCompensation->Character = this;
	//	if (Controller)
	//	{
	//		LagCompensation->Controller = Cast<ABasePlayerController>(Controller);
	//	}
	//}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	{
		DynamicDissolveMaterialInstances.Add(UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(i), this));
		GetMesh()->SetMaterial(i, DynamicDissolveMaterialInstances[i]);
	}

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ThisClass::ReceiveDamage);
	}
	if (AttachedGrenade)
	{
		AttachedGrenade->SetVisibility(false);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateInPlace(DeltaTime);
	HideCameraIfCharacterClose();
	PollInit();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Jump()
{
	if (bDisableGameplay)
	{
		return;
	}
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void ABaseCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();
	if (ElimBotComponent)
	{
		ElimBotComponent->DestroyComponent();
	}

	if(Combat&&
		Combat->EquippedWeapon
		&&(BaseGameMode && BaseGameMode->GetMatchState() != MatchState::InProgress))
	{
		Combat->EquippedWeapon->Destroy();
	}
}

void ABaseCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ABaseCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ABaseCharacter::TurnInPlace(float DeltaTime)
{
	if(AO_Yaw>90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw<-90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if(TurningInPlace!=ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw,0.f,DeltaTime,4.f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw)<15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		}
	}
}

void ABaseCharacter::HideCameraIfCharacterClose()
{
	if(!IsLocallyControlled())
	{
		return;
	}
	if((FollowCamera->GetComponentLocation()-GetActorLocation()).Size()<CameraThreshold)
	{
		GetMesh()->SetVisibility(false);
		if(Combat&&
			Combat->EquippedWeapon&&
			Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (Combat &&
			Combat->EquippedWeapon &&
			Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}

float ABaseCharacter::CalculateSpeed()
{
	return GetVelocity().Size2D();
}

void ABaseCharacter::OnRep_Health(float LastHealth)
{
	UpdateHUDHealth();
	PlayHitReactMontage();
}

void ABaseCharacter::OnRep_Shield(float LastShield)
{
}

void ABaseCharacter::ElimTimerFinished()
{
	if(!BaseGameMode)
	{
		BaseGameMode = GetWorld()->GetAuthGameMode<ABaseGameMode>();
	}
	if(BaseGameMode)
	{
		BaseGameMode->RequestRespawn(this,Controller);
	}
}

void ABaseCharacter::UpdateDissolveMaterial(float DissolveValue)
{
	for (auto& mat : DynamicDissolveMaterialInstances)
	{
		mat->SetScalarParameterValue(MAT_PARAM_DISSOVE, DissolveValue);
	}
}

void ABaseCharacter::StartDissolve()
{
	DissolveTrack.BindDynamic(this,&ThisClass::UpdateDissolveMaterial);
	if(DissolveCurve&& DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve,DissolveTrack);
		DissolveTimeline->Play();
	}
}

void ABaseCharacter::Move(FVector2D Value)
{
	if (bDisableGameplay||
		!Controller||
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
	if (bDisableGameplay ||
		!Combat)
	{
		return;
	}
	if (HasAuthority())
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
	else
	{
		ServerEquipButtonPressed();
	}
}

void ABaseCharacter::CrouchButtonPressed()
{
	if (bDisableGameplay)
	{
		return;
	}
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ABaseCharacter::ReloadButtonPressed()
{
	if ((Combat && Combat->bHoldingTheFlag)||
		bDisableGameplay)
	{
		return;
	}
	if(Combat)
	{
		Combat->Reload();
	}
}

void ABaseCharacter::AimButtonPressed()
{
	if ((Combat && Combat->bHoldingTheFlag)||
		bDisableGameplay)
	{
		return;
	}
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void ABaseCharacter::AimButtonReleased()
{
	if ((Combat && Combat->bHoldingTheFlag) ||
		bDisableGameplay)
	{
		return;
	}
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

void ABaseCharacter::AimOffset(float DeltaTime)
{
	if(Combat&&!Combat->EquippedWeapon)
	{
		return;
	}
	float speed=CalculateSpeed();
	bool isInAir = GetCharacterMovement()->IsFalling();
	if(speed==0.f&&!isInAir)
	{
		bRotateRootBone = true;
		FRotator curAimRot= FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator deltaAimRot= UKismetMathLibrary::NormalizedDeltaRotator(curAimRot,StartingAimRotation);
		AO_Yaw = deltaAimRot.Yaw;
		if(TurningInPlace==ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if(speed>0.f||isInAir)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	CalculateAO_Pitch();
}

void ABaseCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f &&
		!IsLocallyControlled())
	{
		FVector2D inRange(270.f, 360.f);
		FVector2D outRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(inRange, outRange, AO_Pitch);
	}
}

void ABaseCharacter::SimProxiesTurn()
{
	if(!Combat||
		!Combat->EquippedWeapon)
	{
		return;
	}
	bRotateRootBone = false;
	float speed = CalculateSpeed();
	if(speed>0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw=UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation,ProxyRotationLastFrame).Yaw;

	if(FMath::Abs(ProxyYaw)>TurnThreshold)
	{
		if(ProxyYaw>TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold) 
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ABaseCharacter::FireButton(bool bPressed)
{
	if (!Combat ||
		Combat->bHoldingTheFlag ||
		bDisableGameplay)
	{
		return;
	}
	Combat->FireButtonPressed(bPressed);
}

void ABaseCharacter::PlayHitReactMontage()
{
	if (!Combat
		|| !Combat->EquippedWeapon)
	{
		return;
	}
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && HitReactMontage)
	{
		animInstance->Montage_Play(HitReactMontage);
		FName sectionName = SECTION_FROMFRONT;
		animInstance->Montage_JumpToSection(sectionName);
	}
}

void ABaseCharacter::GrenadeButtonPressed()
{
	if (Combat)
	{
		Combat->ThrowGrenade();
	}
}

void ABaseCharacter::DropOrDestroyWeapon(AWeapon* Weapon)
{
}

void ABaseCharacter::DropOrDestroyWeapons()
{
	if (Combat &&
		Combat->EquippedWeapon)
	{
		Combat->EquippedWeapon->Dropped();
	}
	GetWorld()->GetTimerManager().SetTimer(ElimTimer, this, &ThisClass::ElimTimerFinished, ElimDelay);
}

void ABaseCharacter::SetSpawnPoint()
{
}

void ABaseCharacter::OnPlayerStateInitialized()
{
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if(bElimmed)
	{
		return;
	}
	Health = FMath::Clamp(Health- Damage,0.f,MaxHealth);
	UpdateHUDHealth();
	PlayHitReactMontage();

	if(Health==0.f)
	{
		ABaseGameMode* baseGameMode = GetWorld()->GetAuthGameMode<ABaseGameMode>();
		if (baseGameMode)
		{
			ABasePlayerController* attackerController= Cast<ABasePlayerController>(InstigatorController);
			baseGameMode->PlayerEliminated(this, GetBasePlayerController(), attackerController);
		}
	}
}

void ABaseCharacter::PollInit()
{
	if(!BasePlayerState)
	{
		BasePlayerState=GetPlayerState<ABasePlayerState>();
		if (BasePlayerState)
		{
			BasePlayerState->AddToScore(0.f);
			BasePlayerState->AddToDefeats(0);
		}
	}
	if (!BasePlayerController)
	{
		GetBasePlayerController();
		SpawDefaultWeapon();
		UpdateHUDAmmo();
		UpdateHUDHealth();
		UpdateHUDShield();
	}
}

void ABaseCharacter::RotateInPlace(float DeltaTime)
{
	if(bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy &&
		IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
}

void ABaseCharacter::PlayFireMontage(bool bAiming)
{
	if(!Combat
		||!Combat->EquippedWeapon)
	{
		return;
	}
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance&&FireWeaponMontage)
	{
		animInstance->Montage_Play(FireWeaponMontage);
		FName sectionName;
		if(bAiming)
		{
			sectionName = SECTION_RIFLEAIM;
		}
		else
		{
			sectionName = SECTION_RIFLEHIP;
		}
		animInstance->Montage_JumpToSection(sectionName);
	}
}

void ABaseCharacter::PlayReloadMontage()
{
	if (!Combat
		|| !Combat->EquippedWeapon)
	{
		return;
	}
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && ReloadMontage)
	{
		animInstance->Montage_Play(ReloadMontage);
		FName sectionName=FName(UEnum::GetDisplayValueAsText(Combat->EquippedWeapon->GetWeaponType()).ToString());
		animInstance->Montage_JumpToSection(sectionName);
	}
}

void ABaseCharacter::PlayElimMontage()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && ElimMontage)
	{
		animInstance->Montage_Play(ElimMontage);
	}
}

void ABaseCharacter::PlayThrowGrenadeMontage()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && ThrowGrenadeMontage)
	{
		animInstance->Montage_Play(ThrowGrenadeMontage);
	}
}

void ABaseCharacter::PlaySwapMontage()
{
}

void ABaseCharacter::Elim(bool bPlayerLeftGame)
{
	DropOrDestroyWeapons();
	MulticastElim(bPlayerLeftGame);
}

void ABaseCharacter::MulticastElim_Implementation(bool bPlayerLeftGame)
{
	bElimmed = true;
	PlayElimMontage();
	
	for(auto& mat:DynamicDissolveMaterialInstances)
	{
		mat->SetScalarParameterValue(MAT_PARAM_DISSOVE,1.05f);
		mat->SetScalarParameterValue(MAT_PARAM_DISSOVEGLOW,200.f);
	}
	StartDissolve();

	bDisableGameplay = true;
	GetCharacterMovement()->DisableMovement();
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(ElimBotEffect)
	{
		FVector elimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z+200.f);
		ElimBotComponent= UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ElimBotEffect, elimBotSpawnPoint,GetActorRotation());
	}
	if(ElimBotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ElimBotSound, GetActorLocation());
	}
	if(IsLocallyControlled()&&
		Combat&&
		Combat->bAiming&&
		Combat->EquippedWeapon&&
		Combat->EquippedWeapon->GetWeaponType()==EWeaponType::EWT_SniperRifle)
	{
		ShowSniperScopeWidget(false);
	}
}

void ABaseCharacter::UpdateHUDHealth()
{
	if (GetBasePlayerController())
	{
		BasePlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABaseCharacter::UpdateHUDShield()
{
}

void ABaseCharacter::UpdateHUDAmmo()
{
	if (GetBasePlayerController())
	{
		if(GetEquippedWeapon())
		{
			BasePlayerController->SetHUDCarriedAmmo(Combat->CarriedAmmo);
			BasePlayerController->SetHUDWeaponAmmo(GetEquippedWeapon()->GetAmmo(), GetEquippedWeapon()->GetWeaponType());
		}
		else
		{
			BasePlayerController->SetHUDCarriedAmmo(NO_WEAPON);
			BasePlayerController->SetHUDWeaponAmmo(NO_WEAPON);
		}
	}
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

void ABaseCharacter::SetTeamColor(ETeam Team)
{
}

void ABaseCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if(IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ABaseCharacter::IsWeaponEquipped()
{
	return Combat&&Combat->EquippedWeapon;
}

bool ABaseCharacter::IsAiming()
{
	return Combat&&Combat->bAiming;
}

AWeapon* ABaseCharacter::GetEquippedWeapon()
{
	if(!Combat)
	{
		return nullptr;
	}
	return Combat->EquippedWeapon;
}

FVector ABaseCharacter::GetHitTarget() const
{
	if(!Combat)
	{
		return FVector::ZeroVector;
	}
	return Combat->HitTarget;
}

ECombatState ABaseCharacter::GetCombatState() const
{
	if(!Combat)
	{
		return ECombatState::ECS_MAX;
	}
	return Combat->CombatState;
}

bool ABaseCharacter::IsLocallyReloading()
{
	return false;
}

bool ABaseCharacter::IsHoldingTheFlag() const
{
	return false;
}

ETeam ABaseCharacter::GetTeam()
{
	return ETeam();
}

void ABaseCharacter::SetHoldingTheFlag(bool bHolding)
{
}

ABasePlayerController* ABaseCharacter::GetBasePlayerController()
{
	if (!BasePlayerController)
	{
		BasePlayerController = Cast<ABasePlayerController>(Controller);
	}
	return BasePlayerController;
}

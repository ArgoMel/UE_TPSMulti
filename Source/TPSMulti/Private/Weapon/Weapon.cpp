// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon.h"
#include "Character/BaseCharacter.h"
#include "Weapon/Casing.h"
#include "PlayerController/BasePlayerController.h"
#include "Component/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	EnableCustomDepth(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponMesh->MarkRenderStateDirty();

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetSphereRadius(75.f);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidget->SetDrawAtDesiredSize(true);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME_CONDITION(AWeapon, bUseServerSideRewind, COND_OwnerOnly);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if (Owner == nullptr)
	{
		OwnerCharacter = nullptr;
		OwnerController = nullptr;
	}
	else
	{
		if(!OwnerCharacter)
		{
			OwnerCharacter =Cast<ABaseCharacter>(Owner);
		}
		if (OwnerCharacter && 
			OwnerCharacter->GetEquippedWeapon() &&
			OwnerCharacter->GetEquippedWeapon() == this)
		{
			SetHUDAmmo();
		}
	}
}

void AWeapon::OnRep_WeaponState()
{
	OnWeaponStateSet();
}

void AWeapon::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{
	if (HasAuthority())
	{
		return;
	}
	Ammo = ServerAmmo;
	--Sequence;
	Ammo -= Sequence;
	SetHUDAmmo();
}

void AWeapon::ClientAddAmmo_Implementation(int32 AmmoToAdd)
{
	if (HasAuthority())
	{
		return;
	}
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABaseCharacter>(Owner);
	}
	if (OwnerCharacter &&
		OwnerCharacter->GetCombat() &&
		IsFull())
	{
		OwnerCharacter->GetCombat()->JumpToShotgunEnd();
	}
	SetHUDAmmo();
}

void AWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHUDAmmo();
	if (HasAuthority())
	{
		ClientUpdateAmmo(Ammo);
	}
	else
	{
		++Sequence;
	}
}

void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		OnEquipped();
		break;
	case EWeaponState::EWS_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		OnDropped();
		break;
	default: ;
	}
}

void AWeapon::OnEquipped()
{
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(WeaponType==EWeaponType::EWT_SubmachineGun)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	EnableCustomDepth(false);
	
	if(!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	}
	if (OwnerCharacter)
	{
		if (!OwnerController)
		{
			OwnerController = Cast<ABasePlayerController>(OwnerCharacter->Controller);
		}
		if (OwnerController && HasAuthority() &&
			!OwnerController->HighPingDelegate.IsBound())
		{
			OwnerController->HighPingDelegate.AddDynamic(this, &ThisClass::OnPingTooHigh);
		}
	}
}

void AWeapon::OnDropped()
{
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);

	if(!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	}
	if (OwnerCharacter)
	{
		if (!OwnerController)
		{
			OwnerController = Cast<ABasePlayerController>(OwnerCharacter->Controller);
		}
		if (OwnerController && HasAuthority() &&
			OwnerController->HighPingDelegate.IsBound())
		{
			OwnerController->HighPingDelegate.RemoveDynamic(this, &ThisClass::OnPingTooHigh);
		}
	}
}

void AWeapon::OnEquippedSecondary()
{
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
	WeaponMesh->MarkRenderStateDirty();
	if (WeaponType == EWeaponType::EWT_SubmachineGun)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	if(!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	}
	if (OwnerCharacter)
	{
		if (!OwnerController)
		{
			OwnerController = Cast<ABasePlayerController>(OwnerCharacter->Controller);
		}
		if (OwnerController && HasAuthority() &&
			OwnerController->HighPingDelegate.IsBound())
		{
			OwnerController->HighPingDelegate.RemoveDynamic(this, &ThisClass::OnPingTooHigh);
		}
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(OtherActor);

	if (baseCharacter)
	{
		if ((WeaponType == EWeaponType::EWT_Flag && baseCharacter->GetTeam() == Team)||
			baseCharacter->IsHoldingTheFlag()) 
		{
			return;
		}
		baseCharacter->SetOverlappingWeapon(this);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (baseCharacter)
	{
		if ((WeaponType == EWeaponType::EWT_Flag && baseCharacter->GetTeam() == Team)||
			baseCharacter->IsHoldingTheFlag())
		{
			return;
		}
		baseCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::OnPingTooHigh(bool bPingTooHigh)
{
	bUseServerSideRewind=!bPingTooHigh;
}

void AWeapon::SetHUDAmmo()
{
	if(!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	} 
	if (OwnerCharacter)
	{
		if (!OwnerController)
		{
			OwnerController = Cast<ABasePlayerController>(OwnerCharacter->Controller);
		}
		if (OwnerController)
		{
			OwnerController->SetHUDWeaponAmmo(Ammo,WeaponType);
		}
	}
}

void AWeapon::ShowPickupWidget(bool bShowWidget) const
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	if(FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation,false);
	}
	if(CasingClass)
	{
		UWorld* world = GetWorld();
		const USkeletalMeshSocket* ammmoEjectSocket = GetWeaponMesh()->GetSocketByName(SOCKET_AMMOEJECT);
		if (ammmoEjectSocket && world)
		{
			FTransform socketTransform = ammmoEjectSocket->GetSocketTransform(GetWeaponMesh());
			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			world->SpawnActor<ACasing>(CasingClass, socketTransform.GetLocation(), socketTransform.GetRotation().Rotator(), spawnParams);
		}
	}
	SpendRound();
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	const FDetachmentTransformRules detachRules(EDetachmentRule::KeepWorld,true);
	WeaponMesh->DetachFromComponent(detachRules);
	SetOwner(nullptr);
	OwnerCharacter = nullptr;
	OwnerController = nullptr;
}

void AWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	SetHUDAmmo();
	ClientAddAmmo(AmmoToAdd);
}

void AWeapon::TraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets) const
{
	const USkeletalMeshSocket* muzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLEFLASH);
	if (muzzleFlashSocket == nullptr)
	{
		return;
	}
	const FTransform socketTransform = muzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector traceStart = socketTransform.GetLocation();
	const FVector toTargetNormalized = (HitTarget - traceStart).GetSafeNormal();
	const FVector sphereCenter = traceStart + toTargetNormalized * DistanceToSphere;

	for (int32 i = 0; i < NumberOfPellets; ++i)
	{
		const FVector randVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector endLoc = sphereCenter + randVec;
		FVector toEndLoc = endLoc - traceStart;
		toEndLoc = traceStart + toEndLoc * TraceLength / toEndLoc.Size();
		HitTargets.Add(toEndLoc);

		//DrawDebugSphere(GetWorld(), sphereCenter, SphereRadius, 12, FColor::Red, true);
		//DrawDebugSphere(GetWorld(), endLoc, 4.f, 12, FColor::Orange, true);
		//DrawDebugLine(GetWorld(), traceStart, FVector(traceStart + toEndLoc * TraceLength / toEndLoc.Size()), FColor::Cyan, true);
	}

	//const USkeletalMeshSocket* muzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLEFLASH);
	//if (muzzleFlashSocket == nullptr)
	//{
	//	return FVector();
	//}

	//const FTransform socketTransform = muzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	//const FVector traceStart = socketTransform.GetLocation();

	//const FVector toTargetNormalized = (HitTarget - traceStart).GetSafeNormal();
	//const FVector sphereCenter = traceStart + toTargetNormalized * DistanceToSphere;
	//const FVector randVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	//const FVector endLoc = sphereCenter + randVec;
	//const FVector toEndLoc = endLoc - traceStart;

	//return FVector(traceStart + toEndLoc * TraceLength / toEndLoc.Size());
}

void AWeapon::EnableCustomDepth(bool bEnable) const
{
	if(WeaponMesh)
	{
		WeaponMesh->SetRenderCustomDepth(bEnable);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateSet();
}

bool AWeapon::IsEmpty() const
{
	if(Ammo== INF_AMMO)
	{
		return false;
	}
	return Ammo<=0;
}

bool AWeapon::IsFull() const
{
	return Ammo == MagCapacity;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/FlagWeapon.h"

#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"

AFlagWeapon::AFlagWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	GetWeaponMesh()->SetGenerateOverlapEvents(true);
}

void AFlagWeapon::BeginPlay()
{
	Super::BeginPlay();
	InitialTransform = GetActorTransform();

	FLinearColor color=FLinearColor::Black;
	if (GetTeam()==ETeam::ET_BlueTeam)
	{
		color=FLinearColor::Blue;
	}
	else if (GetTeam()==ETeam::ET_RedTeam)
	{
		color=FLinearColor::Red;
	}
	
	for (int32 i = 0; i < GetWeaponMesh()->GetNumMaterials(); ++i)
	{
		DynamicMaterialInstances.Add(UMaterialInstanceDynamic::Create(GetWeaponMesh()->GetMaterial(i), this));
		DynamicMaterialInstances[i]->SetVectorParameterValue(MAT_PARAM_COLOR, color);
		GetWeaponMesh()->SetMaterial(i, DynamicMaterialInstances[i]);
	}
}

void AFlagWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlagWeapon::OnEquipped()
{
	ShowPickupWidget(false);
	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWeaponMesh()->SetSimulatePhysics(false);
	GetWeaponMesh()->SetEnableGravity(false);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECR_Overlap);
	EnableCustomDepth(false);
}

void AFlagWeapon::OnDropped()
{
	Super::OnDropped();
}

void AFlagWeapon::Dropped()
{
	Super::Dropped();
}

void AFlagWeapon::ResetFlag()
{
	if(!OwnerCharacter)
	{
		OwnerCharacter =Cast<ABaseCharacter>(GetOwner());
	}
	if (OwnerCharacter)
	{
		OwnerCharacter->SetHoldingTheFlag(false);
		OwnerCharacter->SetOverlappingWeapon(nullptr);
		OwnerCharacter->UnCrouch();
	}

	if (!HasAuthority())
	{
		return;
	}
	const FDetachmentTransformRules detachRules(EDetachmentRule::KeepWorld,true);
	GetWeaponMesh()->DetachFromComponent(detachRules);
	SetWeaponState(EWeaponState::EWS_Initial);
	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetAreaSphere()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	SetOwner(nullptr);
	OwnerCharacter=nullptr;
	OwnerController=nullptr;

	SetActorTransform(InitialTransform);
}


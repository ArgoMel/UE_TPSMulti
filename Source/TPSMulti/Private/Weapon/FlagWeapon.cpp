// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/FlagWeapon.h"

AFlagWeapon::AFlagWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
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
	Super::OnEquipped();
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
}


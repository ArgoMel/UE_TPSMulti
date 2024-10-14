// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnRep_WeaponState()
{
}

void AWeapon::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{
}

void AWeapon::ClientAddAmmo_Implementation(int32 AmmoToAdd)
{
}

void AWeapon::SpendRound()
{
}

void AWeapon::OnWeaponStateSet()
{
}

void AWeapon::OnEquipped()
{
}

void AWeapon::OnDropped()
{
}

void AWeapon::OnEquippedSecondary()
{
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeapon::OnPingTooHigh(bool bPingTooHigh)
{
}

void AWeapon::SetHUDAmmo()
{
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
}

void AWeapon::Fire(const FVector& HitTarget)
{
}

void AWeapon::Dropped()
{
}

void AWeapon::AddAmmo(int32 AmmoToAdd)
{
}

FVector AWeapon::TraceEndWithScatter(const FVector& HitTarget)
{
	return FVector();
}

void AWeapon::EnableCustomDepth(bool bEnable)
{
}

void AWeapon::SetWeaponState(EWeaponState State)
{
}

bool AWeapon::IsEmpty()
{
	return false;
}

bool AWeapon::IsFull()
{
	return false;
}

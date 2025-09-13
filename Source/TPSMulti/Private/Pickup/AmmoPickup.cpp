// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/AmmoPickup.h"
#include "Character/BaseCharacter.h"
#include "Component/CombatComponent.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	const ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (baseCharacter)
	{
		UCombatComponent* combat = baseCharacter->GetCombat();
		if (combat)
		{
			combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	Destroy();
}

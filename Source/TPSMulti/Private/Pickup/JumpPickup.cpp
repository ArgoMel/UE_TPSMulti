// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/JumpPickup.h"
#include "Character/BaseCharacter.h"
#include "Component/BuffComponent.h"

void AJumpPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (baseCharacter)
	{
		UBuffComponent* buff = baseCharacter->GetBuff();
		if (buff)
		{
			buff->BuffJump(JumpZVelocityBuff, JumpBuffTime);
		}
	}

	Destroy();
}

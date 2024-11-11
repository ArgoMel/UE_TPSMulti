// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ShotgunWeapon.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
//#include "Component/LagCompensationComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"

void AShotgunWeapon::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
}

void AShotgunWeapon::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets)
{
}

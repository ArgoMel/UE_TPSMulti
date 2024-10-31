// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1

#define SOCKET_LEFTHAND FName(TEXT("LeftHandSocket"))
#define SOCKET_RIGHTHAND FName(TEXT("RightHandSocket"))
#define SOCKET_MUZZLEFLASH FName(TEXT("MuzzleFlash"))
#define SOCKET_AMMOEJECT FName(TEXT("AmmoEject"))

#define BONE_RIGHTHAND FName(TEXT("hand_r"))

#define SECTION_RIFLEAIM FName(TEXT("RifleAim"))
#define SECTION_RIFLEHIP FName(TEXT("RifleHip"))
#define SECTION_FROMLEFT FName(TEXT("FromLeft"))
#define SECTION_FROMRIGHT FName(TEXT("FromRight"))
#define SECTION_FROMBACK FName(TEXT("FromBack"))
#define SECTION_FROMFRONT FName(TEXT("FromFront"))

#define MAT_PARAM_DISSOVE FName(TEXT("Dissolve"))
#define MAT_PARAM_DISSOVEGLOW FName(TEXT("DissolveGlow"))

#define PRINT_STRING(MSG)\
if(GEngine)\
{\
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, MSG);\
}

namespace Announcement
{
	const FString NewMatchStartsIn(TEXT("New match starts in:"));
	const FString ThereIsNoWinner(TEXT("There is no winner."));
	const FString YouAreTheWinner(TEXT("You are the winner!"));
	const FString PlayersTiedForTheWin(TEXT("Players tied for the win:"));
	const FString TeamsTiedForTheWin(TEXT("Teams tied for the win:"));
	const FString RedTeam(TEXT("Red team"));
	const FString BlueTeam(TEXT("Blue team"));
	const FString RedTeamWins(TEXT("Red team wins!"));
	const FString BlueTeamWins(TEXT("Blue team wins!"));
}

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DisplayName = "Turning Left"),
	ETIP_Right UMETA(DisplayName = "Turning Right"),
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"),

	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	ET_RedTeam UMETA(DisplayName = "RedTeam"),
	ET_BlueTeam UMETA(DisplayName = "BlueTeam"),
	ET_NoTeam UMETA(DisplayName = "NoTeam"),

	ET_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_ThrowingGrenade UMETA(DisplayName = "Throwing Grenade"),
	ECS_SwappingWeapons UMETA(DisplayName = "Swapping Weapons"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};
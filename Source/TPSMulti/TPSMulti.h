// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

constexpr int32 TopHud = 1;
constexpr float TraceLength = 80000.f;

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
#define ECC_HitBox ECollisionChannel::ECC_GameTraceChannel2

#define SOCKET_PISTOL FName(TEXT("PistolSocket"))
#define SOCKET_LEFTHAND FName(TEXT("LeftHandSocket"))
#define SOCKET_RIGHTHAND FName(TEXT("RightHandSocket"))
#define SOCKET_GRENADE FName(TEXT("GrenadeSocket"))
#define SOCKET_MUZZLEFLASH FName(TEXT("MuzzleFlash"))
#define SOCKET_AMMOEJECT FName(TEXT("AmmoEject"))
#define SOCKET_BACKPACK FName(TEXT("BackpackSocket"))

#define BONE_HEAD FName(TEXT("head"))
#define BONE_PELVIS FName(TEXT("pelvis"))
#define BONE_PELVIS2 FName(TEXT("spine_02"))
#define BONE_PELVIS3 FName(TEXT("spine_03"))
#define BONE_LEFTUPPERARM FName(TEXT("upperarm_l"))
#define BONE_RIGHTUPPERARM FName(TEXT("upperarm_r"))
#define BONE_LEFTLOWERARM FName(TEXT("lowerarm_l"))
#define BONE_RIGHTLOWERARM FName(TEXT("lowerarm_r"))
#define BONE_LEFTHAND FName(TEXT("hand_l"))
#define BONE_RIGHTHAND FName(TEXT("hand_r"))
#define BONE_LEFTTHIGH FName(TEXT("thigh_l"))
#define BONE_RIGHTTHIGH FName(TEXT("thigh_r"))
#define BONE_LEFTCALF FName(TEXT("calf_l"))
#define BONE_RIGHTCALF FName(TEXT("calf_r"))
#define BONE_LEFTFOOT FName(TEXT("foot_l"))
#define BONE_RIGHTFOOT FName(TEXT("foot_r"))

#define SECTION_RIFLEAIM FName(TEXT("RifleAim"))
#define SECTION_RIFLEHIP FName(TEXT("RifleHip"))
#define SECTION_FROMLEFT FName(TEXT("FromLeft"))
#define SECTION_FROMRIGHT FName(TEXT("FromRight"))
#define SECTION_FROMBACK FName(TEXT("FromBack"))
#define SECTION_FROMFRONT FName(TEXT("FromFront"))
#define SECTION_SHOTGUNEND FName(TEXT("ShotgunEnd"))

#define MAT_PARAM_DISSOVE FName(TEXT("Dissolve"))
#define MAT_PARAM_DISSOVEGLOW FName(TEXT("DissolveGlow"))
#define MAT_PARAM_DISSOVECOLOR FName(TEXT("DissolveColor"))
#define MAT_PARAM_TEAMCOLOR FName(TEXT("TeamColor"))

#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

#define INF_AMMO -1
#define NO_WEAPON -2

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
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	EWT_Flag UMETA(DisplayName = "Flag"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};
ENUM_RANGE_BY_COUNT(EWeaponType, EWeaponType::EWT_MAX);

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
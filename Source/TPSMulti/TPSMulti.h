// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#define SOCKET_LEFTHAND FName(TEXT("LeftHandSocket"))
#define SOCKET_RIGHTHAND FName(TEXT("RightHandSocket"))

#define BONE_RIGHTHAND FName(TEXT("hand_r"))

#define PRINT_STRING(MSG)\
if(GEngine)\
{\
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, MSG);\
}
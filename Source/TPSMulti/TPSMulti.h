// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#define PRINT_STRING(MSG)\
if(GEngine)\
{\
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, MSG);\
}
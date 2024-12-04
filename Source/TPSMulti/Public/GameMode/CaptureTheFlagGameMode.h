// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamGameMode.h"
#include "CaptureTheFlagGameMode.generated.h"

UCLASS()
class TPSMULTI_API ACaptureTheFlagGameMode : public ATeamGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController, ABasePlayerController* AttackerController) override;

public:
	//void FlagCaptured(AFlag* Flag, AFlagZone* Zone);
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CaptureTheFlagGameMode.h"

void ACaptureTheFlagGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter,
	ABasePlayerController* VictimController, ABasePlayerController* AttackerController)
{
	ABaseGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

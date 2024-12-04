// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CaptureTheFlagGameMode.h"

#include "CaptureTheFlag/FlagZone.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/FlagWeapon.h"

void ACaptureTheFlagGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController, ABasePlayerController* AttackerController)
{
	ABaseGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

void ACaptureTheFlagGameMode::FlagCaptured(AFlagWeapon* Flag, AFlagZone* Zone) const
{
	//bool bValidCapture=Flag->GetTeam()!=Zone->Team;
	ABaseGameState* gameState=Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
	if (gameState)
	{
		if (Zone->Team==ETeam::ET_BlueTeam)
		{
			gameState->BlueTeamScores();
		}
		else if (Zone->Team==ETeam::ET_RedTeam)
		{
			gameState->RedTeamScores();
		}
	}
}

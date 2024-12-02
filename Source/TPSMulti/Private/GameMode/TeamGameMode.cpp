// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/TeamGameMode.h"

#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/BasePlayerController.h"
#include "PlayerState/BasePlayerState.h"

ATeamGameMode::ATeamGameMode()
{
	bTeamsMatch=true;
}

void ATeamGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ABaseGameState* baseGameState=Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
	if (baseGameState)
	{
		ABasePlayerState* basePlayerState=NewPlayer->GetPlayerState<ABasePlayerState>();
		if (basePlayerState&&
			basePlayerState->GetTeam()==ETeam::ET_NoTeam)
		{
			if (baseGameState->BlueTeam.Num()>=baseGameState->RedTeam.Num())
			{
				baseGameState->RedTeam.AddUnique(basePlayerState);
				basePlayerState->SetTeam(ETeam::ET_RedTeam);
			}
			else
			{
				baseGameState->BlueTeam.AddUnique(basePlayerState);
				basePlayerState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

void ATeamGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ABaseGameState* baseGameState=Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
	ABasePlayerState* basePlayerState=Exiting->GetPlayerState<ABasePlayerState>();
	if (baseGameState&&basePlayerState)
	{
		if (baseGameState->RedTeam.Contains(basePlayerState))
		{
			baseGameState->RedTeam.Remove(basePlayerState);
		}
		else if (baseGameState->BlueTeam.Contains(basePlayerState))
		{
			baseGameState->BlueTeam.Remove(basePlayerState);
		}
	}
}

float ATeamGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	const ABasePlayerState* attackerPlayerState=Attacker->GetPlayerState<ABasePlayerState>();
	const ABasePlayerState* victimPlayerState=Victim->GetPlayerState<ABasePlayerState>();
	if ((!attackerPlayerState||!victimPlayerState)||
		victimPlayerState==attackerPlayerState)
	{
		return BaseDamage;
	}
	if (attackerPlayerState->GetTeam()==victimPlayerState->GetTeam())
	{
		return 0.f;
	}
	return BaseDamage;
}

void ATeamGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController, ABasePlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	if (!AttackerController)
	{
		return;
	}
	ABaseGameState* baseGameState=Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
	ABasePlayerState* attackerPlayerState=AttackerController->GetPlayerState<ABasePlayerState>();
	if (baseGameState&&attackerPlayerState)
	{
		if (attackerPlayerState->GetTeam()==ETeam::ET_BlueTeam)
		{
			baseGameState->BlueTeamScores();
		}
		else if (attackerPlayerState->GetTeam()==ETeam::ET_RedTeam)
		{
			baseGameState->RedTeamScores();
		}
	}
}

void ATeamGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	ABaseGameState* baseGameState=Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
	if (baseGameState)
	{
		for (auto& playerState:baseGameState->PlayerArray)
		{
			ABasePlayerState* basePlayerState=Cast<ABasePlayerState>(playerState.Get());
			if (basePlayerState&&
				basePlayerState->GetTeam()==ETeam::ET_NoTeam)
			{
				if (baseGameState->BlueTeam.Num()>=baseGameState->RedTeam.Num())
				{
					baseGameState->RedTeam.AddUnique(basePlayerState);
					basePlayerState->SetTeam(ETeam::ET_RedTeam);
				}
				else
				{
					baseGameState->BlueTeam.AddUnique(basePlayerState);
					basePlayerState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}
	}
}

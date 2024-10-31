// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerState/BasePlayerState.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Net/UnrealNetwork.h"

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Defeats);
	DOREPLIFETIME(ThisClass, Team);
}

void ABasePlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	if(!Character)
	{
		Character =Cast<ABaseCharacter>(GetPawn());
	}
	if (Character)
	{
		if(!Controller)
		{
			Controller = Cast<ABasePlayerController>(Character->Controller);
		}
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABasePlayerState::OnRep_Team()
{
	if (!Character)
	{
		Character = Cast<ABaseCharacter>(GetPawn());
	}
	if (Character)
	{
		Character->SetTeamColor(Team);
	}
}

void ABasePlayerState::OnRep_Defeats()
{
	if (!Character)
	{
		Character = Cast<ABaseCharacter>(GetPawn());
	}
	if (Character)
	{
		if (!Controller)
		{
			Controller = Cast<ABasePlayerController>(Character->Controller);
		}
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void ABasePlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	if (!Character)
	{
		Character = Cast<ABaseCharacter>(GetPawn());
	}
	if (Character)
	{
		if (!Controller)
		{
			Controller = Cast<ABasePlayerController>(Character->Controller);
		}
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABasePlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	if (!Character)
	{
		Character = Cast<ABaseCharacter>(GetPawn());
	}
	if (Character)
	{
		if (!Controller)
		{
			Controller = Cast<ABasePlayerController>(Character->Controller);
		}
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void ABasePlayerState::SetTeam(ETeam TeamToSet)
{
	Team = TeamToSet;
	if (!Character)
	{
		Character = Cast<ABaseCharacter>(GetPawn());
	}
	if (Character)
	{
		Character->SetTeamColor(Team);
	}
}

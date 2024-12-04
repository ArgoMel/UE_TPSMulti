// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 numberOfPlayers = GameState.Get()->PlayerArray.Num();

	const UGameInstance* gameInstance = GetGameInstance();
	if (gameInstance)
	{
		const UMultiplayerSessionsSubsystem* subsystem = gameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(subsystem);

		if (numberOfPlayers == subsystem->DesiredNumPublicConnections)
		{
			UWorld* world = GetWorld();
			if (world)
			{
				bUseSeamlessTravel = true;

				const FString matchType = subsystem->DesiredMatchType;
				if (matchType == "FreeForAll")
				{
					world->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
				}
				else if (matchType == "Teams")
				{
					world->ServerTravel(FString("/Game/Maps/Teams?listen"));
				}
				else if (matchType == "CaptureTheFlag")
				{
					world->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
				}
			}
		}
	}
}

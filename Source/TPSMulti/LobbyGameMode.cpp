#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if(GameState)
	{
		int32 numberOfPlayers=GameState.Get()->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				1,
				60.f,
				FColor::Yellow,
				FString::Printf(TEXT("플레이어 명수: %d"),numberOfPlayers)
			);

			APlayerState* playerState=NewPlayer->GetPlayerState<APlayerState>();
			if(playerState)
			{
				FString playerName=playerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(
					-1,
					60.f,
					FColor::Cyan,
					FString::Printf(TEXT("%s 게임 참가!"), *playerName)
				);
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (GameState)
	{
		int32 numberOfPlayers = GameState.Get()->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				1,
				60.f,
				FColor::Yellow,
				FString::Printf(TEXT("플레이어 명수: %d"), numberOfPlayers-1)
			);

			APlayerState* playerState = Exiting->GetPlayerState<APlayerState>();
			if (playerState)
			{
				FString playerName = playerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(
					-1,
					60.f,
					FColor::Cyan,
					FString::Printf(TEXT("%s 게임 탈퇴!"), *playerName)
				);
			}
		}
	}
}

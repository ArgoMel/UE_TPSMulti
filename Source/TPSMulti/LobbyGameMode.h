#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class TPSMULTI_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	protected:
		virtual void PostLogin(APlayerController* NewPlayer) override;
		virtual void Logout(AController* Exiting) override;
	
};

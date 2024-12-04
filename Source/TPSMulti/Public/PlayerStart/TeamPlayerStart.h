// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMulti/TPSMulti.h"
#include "GameFramework/PlayerStart.h"
#include "TeamPlayerStart.generated.h"

UCLASS()
class TPSMULTI_API ATeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere)
	ETeam Team;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MocapStickMen.generated.h"

UENUM()
enum class EBodyType : uint8
{
	None,
	Body,
	Hand,

};
UCLASS()
class CYANSDK_API AMocapStickMen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMocapStickMen();

public:
	void randerBoll(FVector point, float DeltaTime, UObject* pt, bool bhand, FColor sColor, FVector apoint);
	void randerStick(FVector from, FVector to, float DeltaTime, UObject* pt);
	void Fulshrander(UObject* pt);
	void drawPoint(FVector point, int  colortype);
	void drawPoint(FVector point, FColor fc);
	void drawPoint(FVector point, FColor fc, float Radius);
	void setPointShow(bool _bshow);
	void setIkPos(bool _bline, float sX, float sY);
	void PosUpdate();
	void drawStick(FVector from, FVector to, UObject* pt, FColor fc);

};

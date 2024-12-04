// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include<Runtime/Online/WebSockets/Public/WebSocketsModule.h>
#include<Runtime/Online/WebSockets/Public/IWebSocket.h>
#include <Kismet/KismetStringLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "MocpFaceModifyCurveData.h"
#include"MocapBaseApi.h"
#include "MocpConstVar.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "MocapLiveFace.generated.h"





UCLASS()
class CYANSDK_API AMocapLiveFace : public ACharacter
{

    GENERATED_BODY()

public:
    AMocapLiveFace();
    void   start(FString tcpip, int32 tcpport);
    void   updataFaceOcc(bool handoccface, bool movefast, bool ankleshift);
    void FacecurvesSetValues();
    void ResetEyeMovements(FFaceChangeValue& FaceWeights);
    void SetStandardEyeValues(FFaceChangeValue& FaceWeights);
    void SetMouthAndJawValues(FFaceChangeValue& FaceWeights);
    static FVector GetModelPelvisVec();
    static bool GetbPelvisMove();
    TArray<FVector> getFaceFirs();
    void UpdatemeshByRec(TArray<FVector> vrec);
    static void ResetExpressDat();
    void FreeSomething();
    bool BodyInit();

};

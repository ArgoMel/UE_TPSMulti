// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "UObject/UObjectIterator.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <shellapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include "MocapBlueprintFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class CYANSDK_API UMocapBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Printf", meta = (WorldContext = "WorldContextObject", DisPlayName = "PrintTraceStack"))
		static void PrintTraceStack(int Depth);
	/** Draw a debug line */
	UFUNCTION(BlueprintCallable, Category = "Rendering", meta = (WorldContext = "WorldContextObject", DisPlayName = "DrawLineSti"))
		static void DrawLineSti(const UObject* uobj, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, float LifeTime, float Thickness);

	/** Draw a debug sphere */
	UFUNCTION(BlueprintCallable, Category = "Rendering", meta = (WorldContext = "WorldContextObject", DisPlayName = "DrawSphereSti"))
		static void DrawSphereSti(const UObject* uobj, FVector const& Center, float Radius, int32 Segments, FColor const& Color, float LifeTime, float Thickness);

	UFUNCTION(BlueprintCallable, Category = "Rendering", meta = (WorldContext = "WorldContextObject", DisPlayName = "FlushPersistentLines"))
		static void FlushPersistentLines(const UObject* uobj);

	UFUNCTION(BlueprintCallable, Category = "IPAddress")
		static FString GetMyIPAddress(bool canBind);

	UFUNCTION(BlueprintCallable, Category = "Log", meta = (DisPlayName = "OutWarningLog"))
		static void OutWarningLog(const FString& str);
	static void OutErrorLog(const FString& str);
	static void OutLog(const FString& str);

	static void AddScreenMessage(const FString& str, float DelayTime = 1.0f, FColor TextColor = FColor::Red);
	//test Func
	static void OutAllSocketLoc(USkeletalMeshComponent* InSkelComp);

	static float GetBoneDistance(USkeletalMeshComponent* InSkelComp, FName FromName, FName ToName);

	static bool IsConfigFileValid(const FString& ConfigPath);

	static float GetIniFaceJawSwingWeightRatio();
	static bool AppendSettingToIni(const FString& Section, const FString& Key, const FString& Value, const FString& Filename);
	static  bool  GetIniCyanhand();
	static FString GetIniIpstr();
	static int GetIniLoopcount();
	static FString GetIniPort();
	static FString GetConfigPath();

	static FString* IntToBooleanString(const int BoolVal);

	static FString* BooleanToString(const bool BoolVal);
	static FString GetEngineVersion();
	static FString GetEngineMajorVersion();

	template<typename ComponentType>
	static inline ComponentType* FindComponent(UWorld* WorldContext)
	{
		if (!WorldContext)
		{
			return nullptr;
		}

		UWorld* TargetWorld = WorldContext;
		for (TObjectIterator<ComponentType> It; It; ++It)
		{
			ComponentType* Component = *It;
			if (IsValid(Component) && Component->GetWorld() == TargetWorld)
			{
				return Component;
			}
		}
		return nullptr;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Extension",meta = (WorldContext = "WorldContextObject", DisPlayName = "LaunchExternalApp"))
	static void LaunchExternalApp(const FString& AppPath, const FString& CommandLineArgs = TEXT(""), bool bLaunchDetached = true, bool bLaunchHidden = false, bool bLaunchReallyHidden = false);
	UFUNCTION(BlueprintCallable, Category = "Extension", meta = (WorldContext = "WorldContextObject", DisPlayName = "LaunchExternalAppWithShellExecute"))
	static void LaunchExternalAppWithShellExecute(const FString& AppPath, int32 ShowCmd);
	//Check whether the character capture initialization is complete
	UFUNCTION(BlueprintPure, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static bool IsPoseInitialized();
	//Conditions for determining whether a person is out of camera range
	UFUNCTION(BlueprintPure, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static bool IsOutCamera();
	//Gets the dynamic capture frame rate
	UFUNCTION(BlueprintPure, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static int GetMocpFPS();
	//Enable recording txt content to local
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static void SetWriteDatawithtxt(bool bvalue);
	//To enable or disable the mirroring function, you need to use it with the mirroring blueprint. ini field: "UseMirror"
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static bool GetISuseMirror();
	//Setting the maximum frame rate limit on the ue is usually used to run a single machine
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static void SetFrameRateLimit();
	//Set the ip address for connecting to the capture
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
	    static void SetCyanIpAddress(FString IPaddress);
	//Sets the bone drawing offset
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static void SetDrawBoneOffset(float offsetX, float offsetY);
	//Enable bone drawing
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static void SetUseBoneDrawing(bool useboneDraw);
	//A process used to kill a task tube
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static void KillExternalProcess(const FString& ProcessName);
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
		static void SetPlaybackSpeed(float speedback=1.0f);
	UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
	    static void SetAssignedKey(FKey assignedkey);
    UFUNCTION(BlueprintCallable, Category = "CyanAnimFunc", meta = (BlueprintThreadSafe))
	static bool IsPelvisMove(float TimeThreshold = 3.0f);

};

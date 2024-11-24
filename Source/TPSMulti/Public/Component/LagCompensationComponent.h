// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

class ABaseCharacter;
class ABasePlayerController;

USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector BoxExtent;

	FBoxInformation()
	: Location()
	, Rotation()
	, BoxExtent()
	{
	}
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;

	UPROPERTY()
	TObjectPtr<ABaseCharacter> Character;

	FFramePackage()
	{
		Time = 0.f;
	}
};

USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHitConfirmed;

	UPROPERTY()
	bool bHeadShot;

	FServerSideRewindResult()
	: bHitConfirmed(false)
	, bHeadShot(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FShotgunServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ABaseCharacter*, uint32> HeadShots;

	UPROPERTY()
	TMap<ABaseCharacter*, uint32> BodyShots;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSMULTI_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	friend class ABaseCharacter;
	ULagCompensationComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
private:
	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY()
	ABasePlayerController* Controller;

	TDoubleLinkedList<FFramePackage> FrameHistory;

	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;

protected:
	void SaveFramePackage(FFramePackage& Package);
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime) const;
	void CacheBoxPositions(ABaseCharacter* HitCharacter, FFramePackage& OutFramePackage) const;
	void MoveBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package) const;
	void ResetHitBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package) const;
	void EnableCharacterMeshCollision(ABaseCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled) const;
	void SaveFramePackage() const;
	FFramePackage GetFrameToCheck(ABaseCharacter* HitCharacter, float HitTime) const;

	//Hitscan
	FServerSideRewindResult ConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation) const;

	//Projectile
	FServerSideRewindResult ProjectileConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime) const;

	//Shotgun
	FShotgunServerSideRewindResult ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart,	const TArray<FVector_NetQuantize>& HitLocations) const;

public:
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color) const;

	//Hitscan
	FServerSideRewindResult ServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime) const;

	//Projectile
	FServerSideRewindResult ProjectileServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity, float HitTime) const;

	//Shotgun
	FShotgunServerSideRewindResult ShotgunServerSideRewind(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime) const;

	UFUNCTION(Server, Reliable)
	void ServerScoreRequest(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);
	void ServerScoreRequest_Implementation(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	UFUNCTION(Server, Reliable)
	void ProjectileServerScoreRequest(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);
	void ProjectileServerScoreRequest_Implementation(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);

	UFUNCTION(Server, Reliable)
	void ShotgunServerScoreRequest(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations, float HitTime);
	void ShotgunServerScoreRequest_Implementation(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations, float HitTime);
};

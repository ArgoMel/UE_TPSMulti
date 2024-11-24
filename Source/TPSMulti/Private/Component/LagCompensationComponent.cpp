// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LagCompensationComponent.h"
#include "Character/BaseCharacter.h"
#include "Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ULagCompensationComponent::ULagCompensationComponent()
	: Character(nullptr)
	, Controller(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(FrameHistory.Num()<=1)
	{
		FFramePackage thisFrame;
		SaveFramePackage(thisFrame);
		FrameHistory.AddHead(thisFrame);
	}
	else
	{
		float historyLength = FrameHistory.GetHead()->GetValue().Time- FrameHistory.GetTail()->GetValue().Time;
		while(historyLength>MaxRecordTime)
		{
			FrameHistory.RemoveNode(FrameHistory.GetTail());
			historyLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		}
		FFramePackage thisFrame;
		SaveFramePackage(thisFrame);
		FrameHistory.AddHead(thisFrame);
		ShowFramePackage(thisFrame,FColor::Red);
	}
}

void ULagCompensationComponent::SaveFramePackage(FFramePackage& Package)
{
	if(!Character)
	{
		Character = Cast<ABaseCharacter>(GetOwner());
	}
	if(Character)
	{
		Package.Time = GetWorld()->GetTimeSeconds();
		for(auto& boxPair: Character->HitCollisionBoxes)
		{
			FBoxInformation boxInformation{};
			boxInformation.Location = boxPair.Value->GetComponentLocation();
			boxInformation.Rotation = boxPair.Value->GetComponentRotation();
			boxInformation.BoxExtent = boxPair.Value->GetScaledBoxExtent();
			Package.HitBoxInfo.Add(boxPair.Key, boxInformation);
		}
	}
}

FFramePackage ULagCompensationComponent::InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime) const
{
	return FFramePackage();
}

void ULagCompensationComponent::CacheBoxPositions(ABaseCharacter* HitCharacter, FFramePackage& OutFramePackage) const
{
}

void ULagCompensationComponent::MoveBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package) const
{
}

void ULagCompensationComponent::ResetHitBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package) const
{
}

void ULagCompensationComponent::EnableCharacterMeshCollision(ABaseCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled) const
{
}

void ULagCompensationComponent::SaveFramePackage() const
{
}

FFramePackage ULagCompensationComponent::GetFrameToCheck(ABaseCharacter* HitCharacter, float HitTime) const
{
	return FFramePackage();
}

FServerSideRewindResult ULagCompensationComponent::ConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation) const
{
	return FServerSideRewindResult();
}

FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime) const
{
	return FServerSideRewindResult();
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations) const
{
	return FShotgunServerSideRewindResult();
}

void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color) const
{
	for(auto& boxInfo:Package.HitBoxInfo)
	{
		DrawDebugBox(GetWorld(),boxInfo.Value.Location,boxInfo.Value.BoxExtent,boxInfo.Value.Rotation.Quaternion(), Color,false,4.f);
	}
}

FServerSideRewindResult ULagCompensationComponent::ServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime) const
{
	return FServerSideRewindResult();
}

FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime) const
{
	return FServerSideRewindResult();
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunServerSideRewind(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime) const
{
	return FShotgunServerSideRewindResult();
}

void ULagCompensationComponent::ServerScoreRequest_Implementation(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
}

void ULagCompensationComponent::ProjectileServerScoreRequest_Implementation(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
}

void ULagCompensationComponent::ShotgunServerScoreRequest_Implementation(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
}

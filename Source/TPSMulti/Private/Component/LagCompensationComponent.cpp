// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LagCompensationComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"

ULagCompensationComponent::ULagCompensationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();
	FFramePackage pakage;
	SaveFramePackage(pakage);
	ShowFramePackage(pakage,FColor::Orange);
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

FFramePackage ULagCompensationComponent::InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime)
{
	return FFramePackage();
}

void ULagCompensationComponent::CacheBoxPositions(ABaseCharacter* HitCharacter, FFramePackage& OutFramePackage)
{
}

void ULagCompensationComponent::MoveBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package)
{
}

void ULagCompensationComponent::ResetHitBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package)
{
}

void ULagCompensationComponent::EnableCharacterMeshCollision(ABaseCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled)
{
}

void ULagCompensationComponent::SaveFramePackage()
{
}

FFramePackage ULagCompensationComponent::GetFrameToCheck(ABaseCharacter* HitCharacter, float HitTime)
{
	return FFramePackage();
}

FServerSideRewindResult ULagCompensationComponent::ConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation)
{
	return FServerSideRewindResult();
}

FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	return FServerSideRewindResult();
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations)
{
	return FShotgunServerSideRewindResult();
}

void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color)
{
	for(auto& boxInfo:Package.HitBoxInfo)
	{
		DrawDebugBox(GetWorld(),boxInfo.Value.Location,boxInfo.Value.BoxExtent,boxInfo.Value.Rotation.Quaternion(), Color,true);
	}
}

FServerSideRewindResult ULagCompensationComponent::ServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	return FServerSideRewindResult();
}

FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	return FServerSideRewindResult();
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunServerSideRewind(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
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

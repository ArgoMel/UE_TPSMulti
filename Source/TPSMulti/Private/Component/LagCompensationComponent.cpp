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
	SaveFramePackage();
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
		Package.Character = Character;
		for(auto& boxPair: Character->HitCollisionBoxes)
		{
			if (!boxPair.Value)
			{
				continue;
			}
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
	const float dist=YoungerFrame.Time-OlderFrame.Time;
	const float interpFraction=FMath::Clamp((HitTime-OlderFrame.Time)/dist,0.f,1.f);
	FFramePackage interpFramePackage;
	interpFramePackage.Time = HitTime;
	for (auto& youngerpair:YoungerFrame.HitBoxInfo)
	{
		const FName& boxInfoName = youngerpair.Key;
		const FBoxInformation& olderbox = OlderFrame.HitBoxInfo[boxInfoName];
		const FBoxInformation& youngerbox = YoungerFrame.HitBoxInfo[boxInfoName];
		
		FBoxInformation interpBoxInfo;
		interpBoxInfo.Location=FMath::VInterpTo(olderbox.Location,youngerbox.Location,1.f,interpFraction);
		interpBoxInfo.Rotation=FMath::RInterpTo(olderbox.Rotation,youngerbox.Rotation,1.f,interpFraction);
		interpBoxInfo.BoxExtent=youngerbox.BoxExtent;

		interpFramePackage.HitBoxInfo.Add(boxInfoName,interpBoxInfo);
	}
	return interpFramePackage;
}

void ULagCompensationComponent::CacheBoxPositions(ABaseCharacter* HitCharacter, FFramePackage& OutFramePackage) const
{
	if (!HitCharacter)
	{
		return;
	}
	for (auto& hitBoxPair:HitCharacter->HitCollisionBoxes)
	{
		if (hitBoxPair.Value)
		{
			FBoxInformation boxInfo;
			boxInfo.Location = hitBoxPair.Value->GetComponentLocation();
			boxInfo.Rotation = hitBoxPair.Value->GetComponentRotation();
			boxInfo.BoxExtent = hitBoxPair.Value->GetScaledBoxExtent();
			OutFramePackage.HitBoxInfo.Add(hitBoxPair.Key,boxInfo);
		}
	}
}

void ULagCompensationComponent::MoveBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package) const
{
	if (!HitCharacter)
	{
		return;
	}
	for (auto& hitBoxPair:HitCharacter->HitCollisionBoxes)
	{
		if (hitBoxPair.Value)
		{
			hitBoxPair.Value->SetWorldLocation(Package.HitBoxInfo[hitBoxPair.Key].Location);
			hitBoxPair.Value->SetWorldRotation(Package.HitBoxInfo[hitBoxPair.Key].Rotation);
			hitBoxPair.Value->SetBoxExtent(Package.HitBoxInfo[hitBoxPair.Key].BoxExtent);
		}
	}
}

void ULagCompensationComponent::ResetHitBoxes(ABaseCharacter* HitCharacter, const FFramePackage& Package) const
{
	if (!HitCharacter)
	{
		return;
	}
	for (auto& hitBoxPair:HitCharacter->HitCollisionBoxes)
	{
		if (hitBoxPair.Value)
		{
			hitBoxPair.Value->SetWorldLocation(Package.HitBoxInfo[hitBoxPair.Key].Location);
			hitBoxPair.Value->SetWorldRotation(Package.HitBoxInfo[hitBoxPair.Key].Rotation);
			hitBoxPair.Value->SetBoxExtent(Package.HitBoxInfo[hitBoxPair.Key].BoxExtent);
			hitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ULagCompensationComponent::EnableCharacterMeshCollision(ABaseCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled) const
{
	if (HitCharacter&&HitCharacter->GetMesh())
	{
		HitCharacter->GetMesh()->SetCollisionEnabled(CollisionEnabled);
	}
}

void ULagCompensationComponent::SaveFramePackage()
{
	if (!Character||
		!Character->HasAuthority())
	{
		return;
	}
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
		//ShowFramePackage(thisFrame,FColor::Red);
	}
}

FFramePackage ULagCompensationComponent::GetFrameToCheck(ABaseCharacter* HitCharacter, float HitTime) const
{
	if (!HitCharacter||
		!HitCharacter->GetLagCompensation()||
		!HitCharacter->GetLagCompensation()->FrameHistory.GetHead()||
		!HitCharacter->GetLagCompensation()->FrameHistory.GetTail())
 	{
 		return FFramePackage();
 	}

 	FFramePackage frameToCheck;
 	bool bShouldInterpolate = true;
 	const TDoubleLinkedList<FFramePackage>& history = HitCharacter->GetLagCompensation()->FrameHistory;
 	const float oldestHistoryTime=history.GetTail()->GetValue().Time;
 	const float newestHistoryTime=history.GetHead()->GetValue().Time;
 	if (oldestHistoryTime>HitTime)
 	{
 		// 랙걸려서 리턴
 		return FFramePackage();
 	}
 	if (oldestHistoryTime==HitTime)
 	{
 		frameToCheck=history.GetTail()->GetValue();
 		bShouldInterpolate=false;
 	}
 	if (newestHistoryTime<=HitTime)
 	{
 		frameToCheck=history.GetHead()->GetValue();
 		bShouldInterpolate=false;
 	}
 	
 	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* younger = history.GetHead();
 	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* older = younger;
 	while (older->GetValue().Time>HitTime)
 	{
 		if (!older->GetNextNode())
 		{
 			break;
 		}
 		older=older->GetNextNode();
 		if (older->GetValue().Time>HitTime)
 		{
 			younger=older;
 		}
 	}
 	if (older->GetValue().Time==HitTime)
 	{
 		frameToCheck=older->GetValue();
 		bShouldInterpolate=false;
 	}
 	if (bShouldInterpolate)
 	{
 		frameToCheck= InterpBetweenFrames(older->GetValue(),younger->GetValue(),HitTime);
 	}
	frameToCheck.Character = HitCharacter;
 	return frameToCheck;
}

FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package, ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime) const
{
	return FServerSideRewindResult();
}

FHitscanServerSideRewindResult ULagCompensationComponent::HitscanConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations) const
{
	for (auto& frame : FramePackages)
	{
		if (!frame.Character)
		{
			return FHitscanServerSideRewindResult();
		}
	}
	FHitscanServerSideRewindResult hitScanResult;
	TArray<FFramePackage> curFrames;
	for (auto& frame : FramePackages)
	{
		FFramePackage curFrame;
		curFrame.Character = frame.Character;
		CacheBoxPositions(frame.Character, curFrame);
		MoveBoxes(frame.Character, frame);
		EnableCharacterMeshCollision(frame.Character, ECollisionEnabled::NoCollision);
		curFrames.Add(curFrame);

		// Enable collision for the head first
		UBoxComponent* HeadBox = frame.Character->HitCollisionBoxes[BONE_HEAD];
		HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
	}
	
	UWorld* world = GetWorld();
	// check for headshots
	for (auto& hitLocation : HitLocations)
	{
		FHitResult confirmHitResult;
		const FVector traceEnd = TraceStart + (hitLocation - TraceStart) * 1.25f;
		if (world)
		{
			world->LineTraceSingleByChannel(confirmHitResult, TraceStart, traceEnd, ECC_HitBox);
			ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(confirmHitResult.GetActor());
			if (baseCharacter)
			{
				if (hitScanResult.HeadShots.Contains(baseCharacter))
				{
					++hitScanResult.HeadShots[baseCharacter];
				}
				else
				{
					hitScanResult.HeadShots.Emplace(baseCharacter, 1);
				}
			}
		}
	}

	// enable collision for all boxes, then disable for head box
	for (auto& frame : FramePackages)
	{
		for (auto& hitBoxPair : frame.Character->HitCollisionBoxes)
		{
			if (hitBoxPair.Value != nullptr)
			{
				hitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				hitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			}
		}
		UBoxComponent* headBox = frame.Character->HitCollisionBoxes[BONE_HEAD];
		headBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// check for body shots
	for (auto& hitLocation : HitLocations)
	{
		FHitResult confirmHitResult;
		const FVector traceEnd = TraceStart + (hitLocation - TraceStart) * 1.25f;
		if (world)
		{
			world->LineTraceSingleByChannel(confirmHitResult, TraceStart, traceEnd, ECC_HitBox);
			ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(confirmHitResult.GetActor());
			if (baseCharacter)
			{
				if (hitScanResult.BodyShots.Contains(baseCharacter))
				{
					++hitScanResult.BodyShots[baseCharacter];
				}
				else
				{
					hitScanResult.BodyShots.Emplace(baseCharacter, 1);
				}
			}
		}
	}

	for (auto& frame : curFrames)
	{
		ResetHitBoxes(frame.Character, frame);
		EnableCharacterMeshCollision(frame.Character, ECollisionEnabled::QueryAndPhysics);
	}
	
	return hitScanResult;
}

void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color) const
{
	for(auto& boxInfo:Package.HitBoxInfo)
	{
		DrawDebugBox(GetWorld(),boxInfo.Value.Location,boxInfo.Value.BoxExtent,boxInfo.Value.Rotation.Quaternion(), Color,false,4.f);
	}
}

FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime) const
{
	return FServerSideRewindResult();
}

FHitscanServerSideRewindResult ULagCompensationComponent::HitscanServerSideRewind(
	const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
	const TArray<FVector_NetQuantize>& HitLocations, float HitTime) const
{
	TArray<FFramePackage> framesToCheck;
	for (ABaseCharacter* hitCharacter : HitCharacters)
	{
		framesToCheck.Add(GetFrameToCheck(hitCharacter, HitTime));
	}

	return HitscanConfirmHit(framesToCheck, TraceStart, HitLocations);
}

void ULagCompensationComponent::ProjectileServerScoreRequest_Implementation(ABaseCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
}

void ULagCompensationComponent::HitscanServerScoreRequest_Implementation(const TArray<ABaseCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
	FHitscanServerSideRewindResult confirm = HitscanServerSideRewind(HitCharacters, TraceStart, HitLocations, HitTime);

	for (auto& hitCharacter : HitCharacters)
	{
		if (!Character||
			!Character->GetEquippedWeapon())
		{
			continue;
		}
		float totalDamage = 0.f;
		if (confirm.HeadShots.Contains(hitCharacter))
		{
			const float headShotDamage = confirm.HeadShots[hitCharacter] * Character->GetEquippedWeapon()->GetHeadShotDamage();
			totalDamage += headShotDamage;
		}
		if (confirm.BodyShots.Contains(hitCharacter))
		{
			const float bodyShotDamage = confirm.BodyShots[hitCharacter] * Character->GetEquippedWeapon()->GetDamage();
			totalDamage += bodyShotDamage;
		}
		UGameplayStatics::ApplyDamage(hitCharacter, totalDamage, Character->Controller, Character->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Component/LagCompensationComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBullet::AProjectileBullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}

#if WITH_EDITOR
void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);

	const FName propertyName = 	Event.GetPropertyName();
	if (propertyName == GET_MEMBER_NAME_CHECKED(AProjectileBullet, InitialSpeed))
	{
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		}
	}
}
#endif

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const ABaseCharacter* ownerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (ownerCharacter)
	{
		ABasePlayerController* ownerController = Cast<ABasePlayerController>(ownerCharacter->Controller);
		if (ownerController)
		{
			if (ownerCharacter->HasAuthority() && !bUseServerSideRewind)
			{
				const float damageToCause = Hit.BoneName == BONE_HEAD ? HeadShotDamage : Damage;

				UGameplayStatics::ApplyDamage(OtherActor, damageToCause, ownerController, this, UDamageType::StaticClass());
				Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
				return;
			}
			ABaseCharacter* hitCharacter = Cast<ABaseCharacter>(OtherActor);
			if (bUseServerSideRewind && 
				ownerCharacter->GetLagCompensation() &&
				ownerCharacter->IsLocallyControlled() &&
				hitCharacter)
			{
				ownerCharacter->GetLagCompensation()->ProjectileServerScoreRequest(hitCharacter, TraceStart, InitialVelocity, ownerController->GetServerTime() - ownerController->SingleTripTime);
			}
		}
	}
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

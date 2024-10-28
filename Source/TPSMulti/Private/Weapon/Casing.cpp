// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	ShellEjectionImpulse = 10.f;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	FVector dir = GetActorForwardVector();
	FRotator randRot(0.f, FMath::RandRange(0.f, 180.f),0.f);
	dir*= randRot.Vector();
	CasingMesh->AddImpulse(dir * ShellEjectionImpulse);
}

void ACasing::Delay_Destroy()
{	
	Destroy();
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ShellSound,GetActorLocation());
	}
	FTimerHandle tempTimer;
	GetWorld()->GetTimerManager().SetTimer(tempTimer,this,&ThisClass::Delay_Destroy,1.f);
}


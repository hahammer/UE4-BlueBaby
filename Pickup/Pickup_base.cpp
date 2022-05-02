// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_base.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainPlayer.h"
// Sets default values
APickup_base::APickup_base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	RootComponent = sphereComp;
	
	sMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sMeshComp"));
	sMeshComp->SetupAttachment(RootComponent);

	particleSysComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particleSysComp"));
	particleSysComp->SetupAttachment(RootComponent);

	particleSys = nullptr;
	souCue = nullptr;

	bIsRotate = false;
	rotateRate = 45;

}

// Called when the game starts or when spawned
void APickup_base::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this,&APickup_base::OnPickupOverlapBegin);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &APickup_base::OnPickupOverlapEnd);
	
}

// Called every frame
void APickup_base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotate)
	{
		FRotator rot = GetActorRotation();
		rot.Yaw += DeltaTime * rotateRate;
		SetActorRotation(rot);
	}

}

void APickup_base::OnPickupOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Super::I am In"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (particleSys)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, particleSys, GetActorLocation(), FRotator(0.0f), true);
			}
			if (souCue)
			{
				UGameplayStatics::PlaySound2D(this, souCue);
			}

		}
	}

}

void APickup_base::OnPickupOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Super::I am out"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
		}
	}
}


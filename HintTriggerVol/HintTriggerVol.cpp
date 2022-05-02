// Fill out your copyright notice in the Description page of Project Settings.


#include "HintTriggerVol.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainPlayer.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AHintTriggerVol::AHintTriggerVol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;

	sMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sMeshComp"));
	sMeshComp->SetupAttachment(RootComponent);

	billComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("billComp"));
	billComp->SetupAttachment(RootComponent);

	widComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widComp"));
	widComp->SetupAttachment(RootComponent);

	hintSound = nullptr;
	widComp->SetVisibility(false);
	widComp->SetWidgetSpace(EWidgetSpace::Screen);
	widComp->SetPivot(FVector2D(0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AHintTriggerVol::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AHintTriggerVol::OnHintBoxOverlapBegin);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AHintTriggerVol::OnHintBoxOverlapEnd);
	if (hintWidgetAsset)
	{
		widComp->SetWidgetClass(hintWidgetAsset);
	}
}

// Called every frame
void AHintTriggerVol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHintTriggerVol::OnHintBoxOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{

			if (hintSound)
			{
				UGameplayStatics::PlaySound2D(this, hintSound);
			}

		}
	}
}

void AHintTriggerVol::OnHintBoxOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
		}
	}
}


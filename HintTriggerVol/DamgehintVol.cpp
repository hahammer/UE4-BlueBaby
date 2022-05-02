// Fill out your copyright notice in the Description page of Project Settings.


#include "DamgehintVol.h"
#include "Character/MainPlayer.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ADamgehintVol::ADamgehintVol()
{
	PrimaryActorTick.bCanEverTick = true;
	damageAmount = 10.0f;
	damageSound = nullptr;
	audioComp = nullptr;
	damageAccumTime = 0.0f;
	damageIntervalTime = 2.0f;
	bHurting = false;
	damageTarget = nullptr;
}

void ADamgehintVol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (damageTarget)
	{
		damageAccumTime += DeltaTime;
		if (bHurting && damageAccumTime > damageIntervalTime)
		{
			if (damType)
			{
				UGameplayStatics::ApplyDamage(damageTarget, damageAmount, nullptr, this, damType);
			}
			damageAccumTime = 0.0f;
		}
	}
}

void ADamgehintVol::OnHintBoxOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnHintBoxOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			damageTarget = mainPlayer;
			bHurting = true;
			GetWidComp()->SetVisibility(true);
			if (damageSound)
			{
				audioComp = UGameplayStatics::SpawnSound2D(GetWorld(), damageSound);
			}
			if (damType)
			{
				UGameplayStatics::ApplyDamage(damageTarget, damageAmount, nullptr, this, damType);
			}

		}
	}
}

void ADamgehintVol::OnHintBoxOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnHintBoxOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			damageTarget = nullptr;
			bHurting = false;
			GetWidComp()->SetVisibility(false);
			if (damageSound)
			{
				audioComp->Stop();
			}
		}
	}
}

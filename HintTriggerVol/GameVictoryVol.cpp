// Fill out your copyright notice in the Description page of Project Settings.


#include "GameVictoryVol.h"
#include "Character/MainPlayer.h"
#include "Components/WidgetComponent.h"

void AGameVictoryVol::OnHintBoxOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnHintBoxOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (mainPlayer->GetHasKey())
			{
				OpenVictoryDoor();
				mainPlayer->GameWin();
			}
			else
			{
				GetWidComp()->SetVisibility(true);
			}

		}
	}
}

void AGameVictoryVol::OnHintBoxOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnHintBoxOverlapEnd(OverlappedComponent,  OtherActor, OtherComp,  OtherBodyIndex);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (mainPlayer->GetHasKey())
			{
				//OpenVictoryDoor();
			}
			else
			{
				GetWidComp()->SetVisibility(false);
			}

		}
	}
}

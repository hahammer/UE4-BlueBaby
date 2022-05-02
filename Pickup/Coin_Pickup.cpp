// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin_Pickup.h"
#include "Character/MainPlayer.h"

void ACoin_Pickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnPickupOverlapBegin( OverlappedComponent,  OtherActor,  OtherComp,  OtherBodyIndex, bFromSweep,  SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("ACoin_Pickup::I am In"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			mainPlayer->IncreaseCoinCount();
			Destroy();
		}
	}
}

void ACoin_Pickup::OnPickupOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("ACoin_Pickup::I am Out"));
}

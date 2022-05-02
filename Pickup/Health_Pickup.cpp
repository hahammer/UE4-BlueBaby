// Fill out your copyright notice in the Description page of Project Settings.


#include "Health_Pickup.h"
#include "Character/MainPlayer.h"


AHealth_Pickup::AHealth_Pickup()
{
	healthAmount = 5.0f;
}
void AHealth_Pickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnPickupOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("AHealth_Pickup::I am In"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			mainPlayer->IncreaseHealth(healthAmount);
			Destroy();
		}
	}
}

void AHealth_Pickup::OnPickupOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("AHealth_Pickup::I am Out"));
}
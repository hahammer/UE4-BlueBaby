// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion_Pickup.h"
#include "Character/MainPlayer.h"
#include "Kismet/GameplayStatics.h"

AExplosion_Pickup::AExplosion_Pickup()
{
	damageAmount = 5.0f;
}

void AExplosion_Pickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnPickupOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("AExplosion_Pickup::I am In"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			//mainPlayer->DecreaseHealth(damageAmount);
			if (damType)
			{
				UGameplayStatics::ApplyDamage(mainPlayer, damageAmount, nullptr, this, damType);
			}
			Destroy();
		}
	}
}

void AExplosion_Pickup::OnPickupOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("AExplosion_Pickup::I am Out"));
}
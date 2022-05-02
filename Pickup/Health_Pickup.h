// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup_base.h"
#include "Health_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class BLUEBABYPROJECT_API AHealth_Pickup : public APickup_base
{
	GENERATED_BODY()
public:
	AHealth_Pickup();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	float healthAmount;
public:
	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup_base.h"
#include "Explosion_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class BLUEBABYPROJECT_API AExplosion_Pickup : public APickup_base
{
	GENERATED_BODY()
public:
	AExplosion_Pickup();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	float damageAmount;

	//±¬Õ¨ÎïÉËº¦½ÇÉ«
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	TSubclassOf<class UDamageType> damType;
public:
	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};

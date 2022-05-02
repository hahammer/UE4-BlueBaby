// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HintTriggerVol/HintTriggerVol.h"
#include "GameVictoryVol.generated.h"

/**
 * 
 */
UCLASS()
class BLUEBABYPROJECT_API AGameVictoryVol : public AHintTriggerVol
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OpenVictoryDoor();

	virtual void OnHintBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnHintBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};

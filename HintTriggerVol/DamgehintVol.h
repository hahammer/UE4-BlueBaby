// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HintTriggerVol/HintTriggerVol.h"
#include "DamgehintVol.generated.h"

/**
 * 
 */
UCLASS()
class BLUEBABYPROJECT_API ADamgehintVol : public AHintTriggerVol
{
	GENERATED_BODY()
public:
	ADamgehintVol();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damgehint)
	float damageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damgehint)
	TSubclassOf<class UDamageType> damType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damgehint)
	class USoundCue* damageSound;
	class UAudioComponent* audioComp;

	float damageAccumTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damgehint)
	float damageIntervalTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damgehint)
	bool bHurting;
	class AMainPlayer* damageTarget;

	virtual void Tick(float DeltaTime) override;

	virtual void OnHintBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnHintBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};

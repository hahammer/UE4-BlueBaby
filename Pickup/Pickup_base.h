// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup_base.generated.h"


class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class BLUEBABYPROJECT_API APickup_base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup_base();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	USphereComponent* sphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	UStaticMeshComponent* sMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	UParticleSystemComponent* particleSysComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	UParticleSystem* particleSys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	USoundCue* souCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	bool bIsRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	float rotateRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup_base.h"
#include "Weapon_Pickup.generated.h"

/**
 * 
 */
class USkeletalMeshComponent;
class UWidgetComponent;
class AMainPlayer;
class UBoxComponent;
class AController;
UCLASS()
class BLUEBABYPROJECT_API AWeapon_Pickup : public APickup_base
{
	GENERATED_BODY()
public:
	AWeapon_Pickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	USkeletalMeshComponent* skeMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	TSubclassOf<UUserWidget> weaponHintWidgetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	USoundCue* equipSound;

	//½ÇÉ«¹¥»÷µÐÈË
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	UBoxComponent* attackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	USoundCue* swingSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	float weaponDamage;

	UFUNCTION(BlueprintCallable)
	void NotifyAnim_ActiveWeaponCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeactiveWeaponCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_WeaponPlayWing();

	void EquipWeapon(AMainPlayer* mainPlayer);

	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	virtual void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//ÎäÆ÷ÉËº¦µÐÈË
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	TSubclassOf<class UDamageType> damType;

	AController* weaponInstigator;
	FORCEINLINE void SetWeaponInstigator(AController* weaponIns) { weaponInstigator = weaponIns; }
protected:
	virtual void BeginPlay() override;
};

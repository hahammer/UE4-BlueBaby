// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLUEBABYPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement)
	float moveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement)
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement)
	APawn* pawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement)
	class AMainPlayer* mainPlayer;

protected:
	virtual void NativeInitializeAnimation() override;

public:
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = AnimProperty)
	void UpdateAnimProperties();
};

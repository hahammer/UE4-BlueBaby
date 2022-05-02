// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MainPlayer.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!pawn)
	{
		pawn = TryGetPawnOwner();
		if (pawn)
		{
			mainPlayer = Cast<AMainPlayer>(pawn);
		}
	}
}

void UPlayerAnimInstance::UpdateAnimProperties()
{
	if (!pawn)
	{
		pawn = TryGetPawnOwner();
	}
	if (pawn)
	{
		if (!mainPlayer)
		{
			mainPlayer = Cast<AMainPlayer>(pawn);
		}
		FVector playerVel = pawn->GetVelocity();
		FVector playerVel2 = FVector(playerVel.X, playerVel.Y,0.0f);
		moveSpeed = playerVel2.Size();

		bIsInAir = pawn->GetMovementComponent()->IsFalling();
	}
}

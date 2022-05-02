// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!pawn)
	{
		pawn = TryGetPawnOwner();
		if (pawn)
		{
			enemy = Cast<AEnemy>(pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimProperties()
{
	if (!pawn)
	{
		pawn = TryGetPawnOwner();
	}
	if (pawn)
	{
		if (!enemy)
		{
			enemy = Cast<AEnemy>(pawn);
		}
		FVector playerVel = pawn->GetVelocity();
		FVector playerVel2 = FVector(playerVel.X, playerVel.Y, 0.0f);
		moveSpeed = playerVel2.Size();

	}
}
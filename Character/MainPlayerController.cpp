// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"





void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (userWidgetAsset)
	{
		playerWidget = CreateWidget(this, userWidgetAsset);
		if (playerWidget)
		{
			playerWidget->AddToViewport();
			playerWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (enemyhealthWidgetAsset)
	{
		enemyhealthWidget = CreateWidget(this, enemyhealthWidgetAsset);
		if (enemyhealthWidget)
		{
			enemyhealthWidget->AddToViewport();
			enemyhealthWidget->SetVisibility(ESlateVisibility::Hidden);
			FVector2D alignInViewport(0.0f,0.0f);
			enemyhealthWidget->SetAlignmentInViewport(alignInViewport);
		}
	}

	if (winGameWidgetAsset)
	{
		winGameWidget = CreateWidget(this, winGameWidgetAsset);
		if (winGameWidget)
		{
			winGameWidget->AddToViewport();
			winGameWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}

	if (loseGameWidgetAsset)
	{
		loseGameWidget = CreateWidget(this, loseGameWidgetAsset);
		if (loseGameWidget)
		{
			loseGameWidget->AddToViewport();
			loseGameWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}

}


void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (enemyhealthWidget)
	{
		bEnemyHealthBarVisible = true;
		enemyhealthWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (enemyhealthWidget)
	{
		bEnemyHealthBarVisible = false;
		enemyhealthWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (enemyhealthWidget)
	{
		FVector2D positionInViewport;
		ProjectWorldLocationToScreen(enemyLocation, positionInViewport);
		positionInViewport.Y -= 80.0f;
		FVector2D sizeInViewport(300.0f,25.0f);
		enemyhealthWidget->SetPositionInViewport(positionInViewport);
		enemyhealthWidget->SetDesiredSizeInViewport(sizeInViewport);
	}
}

void AMainPlayerController::DisplayWinGameWidget()
{
	if (winGameWidget)
	{
		bWinGameWidgetVisible = true;
		winGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveWinGameWidget()
{
	if (winGameWidget)
	{
		bWinGameWidgetVisible = false;
		winGameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayLoseGameWidget()
{
	if (loseGameWidget)
	{
		bLoseGameWidgetVisible = true;
		loseGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveLoseGameWidget()
{
	if (loseGameWidget)
	{
		bLoseGameWidgetVisible = false;
		loseGameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
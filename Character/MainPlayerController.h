// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class BLUEBABYPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> userWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* playerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> enemyhealthWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* enemyhealthWidget;

	FVector enemyLocation;
	FORCEINLINE void SetEnemyLocation(FVector location) { enemyLocation = location; }
	bool bEnemyHealthBarVisible;
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	//游戏胜利或者失败的widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> winGameWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* winGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> loseGameWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* loseGameWidget;

	bool bWinGameWidgetVisible;
	void DisplayWinGameWidget();
	void RemoveWinGameWidget();

	bool bLoseGameWidgetVisible;
	void DisplayLoseGameWidget();
	void RemoveLoseGameWidget();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

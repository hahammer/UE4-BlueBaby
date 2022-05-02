// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class AAIController;
class USphereComponent;
class AMainPlayer;
class UAnimMontage;
class UBoxComponent;
class USoundCue;
class UParticleSystem;
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ES_IDLE UMETA(DisplayName = "Idle"),
	ES_MOVE_TO_TARGET UMETA(DisplayName = "Move_to_target"),
	ES_ATTACK UMETA(DisplayName = "Attack"),
	ES_HURT UMETA(DisplayName = "Hurt"),
	ES_DEAD UMETA(DisplayName = "Dead"),
	ES_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class BLUEBABYPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	EEnemyState enemyState;

	FORCEINLINE void SetEnemyState(EEnemyState state) { enemyState = state; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	AAIController* enemyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	USphereComponent* agroSphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	USphereComponent* attackSphereComp;

	//追逐和攻击角色玩家
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bInAgroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bInAttackSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	AMainPlayer* playerTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
	UAnimMontage* attackMontage;

	float acuTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float tickChaseTime;

	void Enemy_Attack();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_AttackEnd();
	//与玩家角色进行打斗

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UBoxComponent* attackComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	float health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float enemyDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	USoundCue* hitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	USoundCue* swingSound;

	UFUNCTION(BlueprintCallable)
	void NotifyAnim_ActiveEnemyCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeactiveEnemyCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_EnemyPlayWing();

	//敌人延迟进攻
	FTimerHandle attackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float minAttackTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float maxAttackTime;

	//敌人伤害角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TSubclassOf<class UDamageType> damType;

	//敌人受到伤害
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Die();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeathEnd();
	FTimerHandle deathTimer;
	float deathDelay;
	void DestroyCall();
	bool EnemyAlive();
	bool bHasValidTarget;
	FORCEINLINE void SetHasValidTarget(bool has) { bHasValidTarget = has; }

	//敌人漫游
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =AI )
	bool bCanWander;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bWandering;
	float timePause;
	float timeAccumWander;
	void FindSphereRandomLocation(FVector& randLocation);

   //敌人受伤
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bHurting;
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_HurtingEnd();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float launchXY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float launchZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	USoundCue* deathSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnAgroSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnAgroSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnAttackSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnAttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void MoveToTarget(AMainPlayer* target);
};

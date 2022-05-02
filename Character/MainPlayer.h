// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"


class AWeapon_Pickup;
class APickup_base;
class USoundCue;
class UParticleSystem;

UENUM(BlueprintType)
enum class EMainPlayerState : uint8
{
	MPS_NORMAL UMETA(DisplayName = "Normal"),
	MPS_SPRINTING UMETA(DisplayName = "Sprinting"),
	MPS_ATTACK UMETA(DisplayName = "Attack"),
	MPS_HURT UMETA(DisplayName = "Hurt"),
	MPS_DEAD UMETA(DisplayName = "Dead"),
	MPS_MAX UMETA(DisplayName = "DefaultMax")
};


UCLASS()
class BLUEBABYPROJECT_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

private:
		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	// Sets default values for this character's properties
	AMainPlayer();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


	//��ɫ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	EMainPlayerState mainPlayerState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	float health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	int32 coinCount;

	void SetMainPlayerState(EMainPlayerState state);
	void IncreaseHealth(float amount);
	void DecreaseHealth(float amount);
	void IncreaseCoinCount();
	void Die();

	//������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	float stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float maxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float sprintLoss;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	float runningSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	float sprintingSpeed;


	void IncreaseStamina(float amount);
	void DecreaseStamina(float amount);

	bool bShiftdown;
	void BSprintDown();
	void BSprintUp();

	bool bForwardMove;
	bool bRightMove;

	//�������
	bool bLMBDown;
	void LMouseDown();
	void LMouseUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	AWeapon_Pickup* weaponHold;

	APickup_base* pickupOverlap;

	void SetWeaponHold(AWeapon_Pickup* weapon);
	FORCEINLINE void SetPickupOverlap(APickup_base* pickup) { pickupOverlap = pickup; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//��ɫ����
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	virtual void Jump() override;

public:
	//��ɫ����
	bool bAttacking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack)
	class UAnimMontage* attackMontage;
	void Attack();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_AttackingEnd();

	//����˽�ɫ���д�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	USoundCue* hitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	UParticleSystem* hitParticle;

	//��ɫ����ʱת�����
	float interpSpeed;
	bool bInterpToEnemy;
	FORCEINLINE void SetInterpToEnemy(bool interpBool) {bInterpToEnemy = interpBool;}
	FRotator GetLookAtRotationYaw(FVector targetVector);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	class AEnemy* enemyTarget;
	FORCEINLINE void SetEnemyTarget(class AEnemy* target) { enemyTarget = target; }

	//��ɫ�ܵ��˺�
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeathEnd();
	bool PlayerAlive();

	//Ϊ���˽�ɫ��ʾѪ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	bool bHasEnemyTarget;
	FORCEINLINE void SetHasEnemyTarget(bool hasTarget) { bHasEnemyTarget = hasTarget; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	class AMainPlayerController* mainPlayerController;
	FVector enemeyTargetLocation;

	//���µ���Ŀ��
	void UpdateEnemyTarget();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<AEnemy> enemyFilter;

	//��ɫ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	class UMaterialInstanceDynamic* hurtMatInst;
	FTimerHandle hurtTimer;
	void ResetMatInst();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	bool bHurting;
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_HurtingEnd();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float launchXY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float launchZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	class USoundCue* deathSound;
	FTimerHandle DeactiveCollisionTimer;
	void DeactiveCapuseCollision();

	//��ɫ������ʱ����
	float coolAttackTime;
	float coolAttackAccumTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	bool bCoolEndAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	bool bKeepAttacking;

	//��Ϸ��ʤ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	int allCoinNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	TSubclassOf<AActor> coinActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool bHasKey;
	FORCEINLINE void SetHasKey(bool has) { bHasKey = has; }
	FORCEINLINE bool GetHasKey() { return bHasKey; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	int keyNum;
	void SetKeyFunc();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	TSubclassOf<AActor> targetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	TSubclassOf<AActor> keyActor;
	FVector targetLocation;
	void SpawnKeyActor();

	//��Ϸʤ����ʧ������߼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool bHasGameWin;
	FORCEINLINE void SetHasGameWin(bool has) { bHasGameWin = has; }
	FORCEINLINE bool GetHasGameWin() { return bHasGameWin; }
	FTimerHandle winTimerhandle;
	FTimerHandle loseTimerhandle;
	void GameWin();
	void GameLose();
	void ShowWinWidget();
	void ShowLoseWidget();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	class USoundCue* winMusic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	class USoundCue* loseMusic;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

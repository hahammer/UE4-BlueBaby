// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Pickup/Weapon_Pickup.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/MainPlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(34.f, 76.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	mainPlayerState = EMainPlayerState::MPS_NORMAL;
	maxHealth = 100.0f;
	health = maxHealth;
	coinCount = 0;

	//加速跑
	maxStamina = 100.0f;
	stamina = maxStamina;
	sprintLoss = 5.0f;
	runningSpeed = GetCharacterMovement()->MaxWalkSpeed;
	sprintingSpeed = runningSpeed + 300.0f;
	bShiftdown = false;
	bForwardMove = false;
	bRightMove = false;
	//武器相关
	bLMBDown = false;
	weaponHold = nullptr;
	pickupOverlap = nullptr;

	//角色攻击
	bAttacking = false;
	attackMontage = nullptr;

	hitSound = nullptr;
	hitParticle = nullptr;

	//角色攻击时转向敌人
	interpSpeed = 15.0f;
	bInterpToEnemy = false;
	enemyTarget = nullptr;

	//为敌人角色添加血量
	bHasEnemyTarget = false;

	//角色受伤
	bHurting = false;
	launchXY = 400.0f;
	launchZ = 150.0f;
	deathSound = nullptr;

	//角色进攻有时间间隔
	coolAttackTime = 1.0f;
	coolAttackAccumTime = 0.0f;
	bCoolEndAttack = true;
	bKeepAttacking = false;

	//游戏的胜利法则
	allCoinNum = 0;
	bHasKey = false;
	keyNum = 0;
	targetLocation = FVector(0.0f, 0.0f, 0.0f);

	//游戏胜利或失败相关逻辑
	bHasGameWin = false;
	winMusic = nullptr;
	loseMusic = nullptr;
}


// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	mainPlayerController = Cast<AMainPlayerController>(GetController());
	hurtMatInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	coolAttackTime = FMath::RandRange(0.2f,0.8f);

	//获取场景中所有的金币
	if (coinActor)
	{
		TArray<AActor*> coinArray;
		UGameplayStatics::GetAllActorsOfClass(this,coinActor, coinArray);
		if (coinArray.Num() > 0)
		{
			allCoinNum = coinArray.Num();
		}
	}

	//获取场景中的targetPoint
	if (targetActor)
	{
		TArray<AActor*> targetArray;
		UGameplayStatics::GetAllActorsOfClass(this, targetActor, targetArray);
		if (targetArray.Num() > 0)
		{
			targetLocation = targetArray[0]->GetActorLocation();
		}
	}

}

void AMainPlayer::MoveForward(float Value)
{
	if (!(PlayerAlive())) return;
	bForwardMove = false;

	if ((Controller != NULL) && (Value != 0.0f) && (!bAttacking) &&(!bHurting))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		bForwardMove = true;
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if (!(PlayerAlive())) return;
	bRightMove = false;

	if ((Controller != NULL) && (Value != 0.0f)&&(!bAttacking) && (!bHurting))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		bRightMove = true;
	}
}

void AMainPlayer::TurnAtRate(float Rate)
{
	if (Rate)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}

}

void AMainPlayer::LookUpAtRate(float Rate)
{
	if (Rate)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}

}

void AMainPlayer::Jump()
{
	if (!(PlayerAlive()) || ( bHurting))
	{
		return;
	}

	Super::Jump();
}




// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerAlive())
	{
		if (bShiftdown)
		{
			if (bForwardMove || bRightMove)
			{
				if (stamina >= 1.0f)
				{
					SetMainPlayerState(EMainPlayerState::MPS_SPRINTING);
					DecreaseStamina(sprintLoss * DeltaTime);
				}
				else
				{
					SetMainPlayerState(EMainPlayerState::MPS_NORMAL);
				}
			}
			else
			{
				SetMainPlayerState(EMainPlayerState::MPS_NORMAL);
			}
		}
		else
		{
			SetMainPlayerState(EMainPlayerState::MPS_NORMAL);
			IncreaseStamina(sprintLoss * DeltaTime);
		}

		//角色攻击时转向敌人
		if (bInterpToEnemy &&  enemyTarget )
		{
			FRotator rotTarget = GetLookAtRotationYaw(enemyTarget->GetActorLocation());
			FRotator rotTargetPlayer = FMath::RInterpTo(GetActorRotation(), rotTarget, DeltaTime, interpSpeed);
			SetActorRotation(rotTargetPlayer);
		}

		if (bHasEnemyTarget && enemyTarget)
		{
			enemeyTargetLocation = enemyTarget->GetActorLocation();
			if (mainPlayerController)
			{
				mainPlayerController->SetEnemyLocation(enemeyTargetLocation);
			}
		}

		//角色进攻有时间间隔
		if (!bCoolEndAttack)
		{
			coolAttackAccumTime += DeltaTime;
			if (coolAttackAccumTime > coolAttackTime)
			{
				bCoolEndAttack = true;
				coolAttackAccumTime = 0.0f;
				coolAttackTime = FMath::RandRange(0.2f, 0.8f);
			}
		}

		//角色持续进攻
		if (bKeepAttacking)
		{
			Attack();
		}

		
	}



}

FRotator AMainPlayer::GetLookAtRotationYaw(FVector targetVector)
{
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),targetVector);
	FRotator rotYaw = FRotator(0.0f,rot.Yaw,0.0f);
	return rotYaw;
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprinting", IE_Pressed, this, &AMainPlayer::BSprintDown);
	PlayerInputComponent->BindAction("Sprinting", IE_Released, this, &AMainPlayer::BSprintUp);

	PlayerInputComponent->BindAction("LMouseDown", IE_Pressed, this, &AMainPlayer::LMouseDown);
	PlayerInputComponent->BindAction("LMouseDown", IE_Released, this, &AMainPlayer::LMouseUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainPlayer::LookUpAtRate);
}

void AMainPlayer::SetMainPlayerState(EMainPlayerState state)
{
	mainPlayerState = state;
	if (mainPlayerState == EMainPlayerState::MPS_SPRINTING)
	{
		GetCharacterMovement()->MaxWalkSpeed = sprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
	}
}

void AMainPlayer::IncreaseHealth(float amount)
{
	if (health + amount >= maxHealth)
	{
		health = maxHealth;
	}
	else
	{
		health += amount;
	}
}

void AMainPlayer::DecreaseHealth(float amount)
{
	if (health - amount <= 0.0f)
	{
		health = 0.0f;
		Die();
	}
	else
	{
		health -= amount;
	}
}

void AMainPlayer::IncreaseCoinCount()
{
	coinCount += 1;
	if (coinCount == allCoinNum)
	{
		SpawnKeyActor();
	}

}

void AMainPlayer::Die()
{
	//UE_LOG(LogTemp, Warning, TEXT("AMainPlayer::Die"));
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (animInst && attackMontage)
	{
		animInst->Montage_Play(attackMontage);
		animInst->Montage_JumpToSection(FName("Death"), attackMontage);
	}

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(DeactiveCollisionTimer, this, &AMainPlayer::DeactiveCapuseCollision, 0.8f);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (weaponHold)
	{
		weaponHold->sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponHold->sMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponHold->skeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponHold->attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (deathSound)
	{
		UGameplayStatics::PlaySound2D(this, deathSound);
	}

	GameLose();
}

void AMainPlayer::IncreaseStamina(float amount)
{
	if (stamina + amount >= maxStamina)
	{
		stamina = maxStamina;
	}
	else
	{
		stamina += amount;
	}
}

void AMainPlayer::DecreaseStamina(float amount)
{
	if (stamina - amount <= 0.0f)
	{
		stamina = 0.0f;
	}
	else
	{
		stamina -= amount;
	}
}

void AMainPlayer::BSprintDown()
{
	bShiftdown = true;
}

void AMainPlayer::BSprintUp()
{
	bShiftdown = false;
}

void AMainPlayer::LMouseDown()
{
	bLMBDown = true;
	if (pickupOverlap &&(PlayerAlive()))
	{
		AWeapon_Pickup* weapon = Cast<AWeapon_Pickup>(pickupOverlap);
		weapon->EquipWeapon(this);
	}
	else if (weaponHold)
	{
		Attack();
	}
}

void AMainPlayer::LMouseUp()
{
	bLMBDown = false;
}

void AMainPlayer::SetWeaponHold(AWeapon_Pickup * weapon)
{
	if (weaponHold)
	{
		weaponHold->Destroy();
	}
	weaponHold = weapon;
}

void AMainPlayer::Attack()
{
	if (!bAttacking 	&&PlayerAlive() &&(!bHurting)&& bCoolEndAttack)
	{
		bAttacking = true;
		SetInterpToEnemy(true);
		UAnimInstance* animInst = GetMesh()->GetAnimInstance();
		if (animInst && attackMontage)
		{
			int32 section = FMath::RandRange(0, 1);
			switch (section)
			{
			case 0:
				animInst->Montage_Play(attackMontage);
				animInst->Montage_JumpToSection(FName("Attack_1"), attackMontage);
				break;
			case 1:
				animInst->Montage_Play(attackMontage);
				animInst->Montage_JumpToSection(FName("Attack_2"), attackMontage);
				break;
			default:
				break;
			}
		}
	}

}

void AMainPlayer::NotifyAnim_AttackingEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	bCoolEndAttack = false;
	if (bLMBDown)
	{
		//Attack();
		bKeepAttacking = true;
	}
	else
	{
		bKeepAttacking = false;
	}
}

float AMainPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (health - Damage <= 0.0f)
	{
		health = 0.0f;
		SetMainPlayerState(EMainPlayerState::MPS_DEAD);
		bHurting = false;
		bAttacking = false;
		SetInterpToEnemy(false);
		if (mainPlayerController)
		{
			mainPlayerController->RemoveEnemyHealthBar();
		}
		if (DamageCauser)
		{
			AEnemy* enmey = Cast<AEnemy>(DamageCauser);
			if (enmey)
			{
				enmey->bHasValidTarget = false;
			}
		}

		Die();
		GetWorldTimerManager().ClearTimer(hurtTimer);
	}
	else
	{
		//if (!bHurting)
		//{
			health -= Damage;
			hurtMatInst->SetScalarParameterValue(FName("HurtFlag"), 1.0f);
			GetWorldTimerManager().SetTimer(hurtTimer, this, &AMainPlayer::ResetMatInst, 0.8f);

			if (DamageCauser &&PlayerAlive())
			{
				AEnemy* enemy = Cast<AEnemy>(DamageCauser);
				if (enemy)
				{
					bHurting = true;
					bAttacking = false;
					SetInterpToEnemy(false);
					FVector launchDirection = enemy->GetActorForwardVector();
					launchDirection.X = launchDirection.X*launchXY;
					launchDirection.Y = launchDirection.Y*launchXY;
					launchDirection.Z = launchDirection.Z + launchZ;
					LaunchCharacter(launchDirection, true, true);
					UAnimInstance* animInst = GetMesh()->GetAnimInstance();
					if (animInst && attackMontage)
					{
						animInst->Montage_Play(attackMontage);
						animInst->Montage_JumpToSection(FName("Hurt"), attackMontage);
					}
				}
			}
		//}

	}

	return Damage;
}

void AMainPlayer::NotifyAnim_DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

bool AMainPlayer::PlayerAlive()
{
	return mainPlayerState != EMainPlayerState::MPS_DEAD;
}

void AMainPlayer::UpdateEnemyTarget()
{
	TArray<AActor*> overlapActors;
	GetOverlappingActors(overlapActors, enemyFilter);
	if (overlapActors.Num() == 0)
	{
		SetEnemyTarget(nullptr);
		SetHasEnemyTarget(false);
		if (mainPlayerController)
		{
			mainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}
	AEnemy* closetEnemy = Cast<AEnemy>(overlapActors[0]);
	if (closetEnemy)
	{
		FVector playerLocation = GetActorLocation();
		float minDistance = (playerLocation - closetEnemy->GetActorLocation()).Size();
		for (auto actor : overlapActors)
		{
			AEnemy* actorEnmey = Cast<AEnemy>(actor);
			if (actorEnmey)
			{
				float distance = (playerLocation - actorEnmey->GetActorLocation()).Size();
				if (distance < minDistance)
				{
					minDistance = distance;
					closetEnemy = actorEnmey;
				}
			}
		}
		if (mainPlayerController)
		{
			mainPlayerController->DisplayEnemyHealthBar();
		}

		SetEnemyTarget(closetEnemy);
		SetHasEnemyTarget(true);
	}
}

void AMainPlayer::ResetMatInst()
{
	hurtMatInst->SetScalarParameterValue(FName("HurtFlag"), 0.0f);
}

void AMainPlayer::NotifyAnim_HurtingEnd()
{
	bHurting = false;
	if (bLMBDown)
	{
		//Attack();
		bKeepAttacking = true;
	}
	else
	{
		bKeepAttacking = false;
	}
}

void AMainPlayer::DeactiveCapuseCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::SetKeyFunc()
{
	keyNum += 1;
	SetHasKey(true);
}

void AMainPlayer::SpawnKeyActor()
{
	if (keyActor)
	{
		FActorSpawnParameters spawnParam;
		GetWorld()->SpawnActor<AActor>(keyActor,targetLocation,FRotator(0.0f), spawnParam);
	}
}

void AMainPlayer::GameWin()
{
	SetHasGameWin(true);
	DisableInput(mainPlayerController);
	float timeDelay = 2.5f;
	GetWorldTimerManager().SetTimer(winTimerhandle,this,&AMainPlayer::ShowWinWidget, timeDelay);
	GetWorldTimerManager().ClearTimer(loseTimerhandle);
}

void AMainPlayer::GameLose()
{
	SetHasGameWin(false);
	DisableInput(mainPlayerController);
	float timeDelay = 2.5f;
	GetWorldTimerManager().SetTimer(loseTimerhandle, this, &AMainPlayer::ShowLoseWidget, timeDelay);
	GetWorldTimerManager().ClearTimer(winTimerhandle);
}

void AMainPlayer::ShowWinWidget()
{
	mainPlayerController->DisplayWinGameWidget();
	mainPlayerController->bShowMouseCursor = true;
	if (winMusic)
	{
		UGameplayStatics::PlaySound2D(this, winMusic);
	}
}

void AMainPlayer::ShowLoseWidget()
{
	mainPlayerController->DisplayLoseGameWidget();
	mainPlayerController->bShowMouseCursor = true;
	if (loseMusic)
	{
		UGameplayStatics::PlaySound2D(this, loseMusic);
	}
}

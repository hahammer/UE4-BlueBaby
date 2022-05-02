// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "Character/MainPlayer.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Character/MainPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	agroSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("agroSphereComp"));
	agroSphereComp->SetupAttachment(GetRootComponent());
	agroSphereComp->InitSphereRadius(600.0f);

	attackSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("attackSphereComp"));
	attackSphereComp->SetupAttachment(GetRootComponent());
	attackSphereComp ->InitSphereRadius(75.0f);

	attackComp = CreateDefaultSubobject<UBoxComponent>(TEXT("attackComp"));
	//attackComp->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("enemySocket"));
	attackComp->SetupAttachment(GetMesh(), FName("enemySocket"));

	GetCharacterMovement()->MaxWalkSpeed = 400;
	enemyState = EEnemyState::ES_IDLE;

	//追逐和攻击角色玩家
	bInAgroSphere = false;
	bInAttackSphere = false;
	bAttacking = false;
	attackMontage = nullptr;
	playerTarget = nullptr;
	acuTime = 0.0f;
	tickChaseTime = 2.0f;

	//与玩家角色进行打斗
	maxHealth = 100.0f;
	health = maxHealth;
	enemyDamage = 20.0f;
	hitSound = nullptr;
	hitParticle = nullptr;
	swingSound = nullptr;
	//敌人延迟进攻
	minAttackTime = 0.5f;
	maxAttackTime = 3.5f;
	//敌人受到伤害
	deathDelay = 2.0f;
	bHasValidTarget = false;

	//敌人漫游
	bCanWander = false;
	bWandering = true;
	timePause = 2.0f;
	timeAccumWander = 0.0f;

	//敌人受伤
	bHurting = false;
	launchXY = 400.0f;
	launchZ = 150.0f;
	deathSound = nullptr;
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	enemyController = Cast<AAIController>(GetController());
	agroSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAgroSphereOverlapBegin);
	agroSphereComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAgroSphereOverlapEnd);
	attackSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackSphereOverlapBegin);
	attackSphereComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackSphereOverlapEnd);
	
	attackComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackOverlapBegin);
	attackComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackOverlapEnd);

	attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	attackComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	attackComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//设置跟相机的碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//敌人漫游随机种子
	double timeMs = FTimespan(FDateTime::Now().GetTicks()).GetTotalMilliseconds();
	int32 timeSeed = (int32)(((int64)timeMs)%INT_MAX);
	FMath::RandInit(timeSeed);

	timePause = FMath::RandRange(2.0f,4.0f);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//修复追逐角色
	acuTime += DeltaTime;
	if (acuTime > tickChaseTime)
	{
		acuTime = 0.0f;
		if (enemyController && EnemyAlive())
		{
			if (playerTarget)
			{
				if (playerTarget->PlayerAlive())
				{
					if (bInAgroSphere && !bInAttackSphere && !bAttacking &&enemyState == EEnemyState::ES_MOVE_TO_TARGET && (GetCharacterMovement()->Velocity.Size() < 10.0f))
					{
						//UE_LOG(LogTemp, Warning, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXX!"))
						MoveToTarget(playerTarget);
					}
				}

			}
		}
	}

	//敌人漫游
	timeAccumWander += DeltaTime;
	if (bCanWander &&EnemyAlive())
	{
		if (bWandering && timeAccumWander > timePause && !bInAgroSphere && !bInAttackSphere)
		{
			SetEnemyState(EEnemyState::ES_IDLE);
			if (enemyController)
			{
				FVector rdLocation;
				FindSphereRandomLocation(rdLocation);
				enemyController->MoveToLocation(rdLocation,4.0f);
			}
			timeAccumWander = 0.0f;
			timePause = FMath::RandRange(2.0f, 4.0f);
		}
	}

}



void AEnemy::FindSphereRandomLocation(FVector& randLocation)
{
	FVector enemyLocation = GetActorLocation();
	float radius = FMath::RandRange(200.0f,1000.0f);
	float angle = FMath::RandRange(0.0f, 360.0f);
	float deltaX = radius * FMath:: Cos(FMath::DegreesToRadians(angle));
	float deltaY = radius * FMath::Sin(FMath::DegreesToRadians(angle));
	randLocation = FVector(enemyLocation.X+deltaX, enemyLocation.Y + deltaY, enemyLocation.Z);

}



// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnAgroSphereOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && EnemyAlive())
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			if (!(main->PlayerAlive()) || main->GetHasGameWin())  return;
			playerTarget = main;
			bInAgroSphere = true;
			bWandering = false;
			MoveToTarget(main);
		}
	}
}

void AEnemy::OnAgroSphereOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && EnemyAlive() )
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			bHasValidTarget = false;
			if (!(main->PlayerAlive()) || main->GetHasGameWin())  return;
			if (main->enemyTarget == this)
			{
				main->SetHasEnemyTarget(false);
				main->SetEnemyTarget(nullptr);
			}
			playerTarget = nullptr;
			bInAgroSphere = false;
			bWandering = true;
			if (!bCanWander)
			{
				if (enemyController)
				{
					SetEnemyState(EEnemyState::ES_IDLE);
					enemyController->StopMovement();
				}
			}

		}
	}
}

void AEnemy::OnAttackSphereOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && EnemyAlive() )
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			bHasValidTarget = true;
			if (!(main->PlayerAlive()) || main->GetHasGameWin())  return;
			//main->SetHasEnemyTarget(true);
			//main->SetEnemyTarget(this);
			//if (main->mainPlayerController)
			//{
			//	main->mainPlayerController->DisplayEnemyHealthBar();
			//}
			main->UpdateEnemyTarget();
			bInAttackSphere = true;
			Enemy_Attack();
		}
	}
}

void AEnemy::OnAttackSphereOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && EnemyAlive() )
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			if (!(main->PlayerAlive()) || main->GetHasGameWin())  return;
			if (main->enemyTarget ==this)
			{
				main->SetHasEnemyTarget(false);
				main->SetEnemyTarget(nullptr);
				main->UpdateEnemyTarget();
				if (main->mainPlayerController)
				{
					main->mainPlayerController->RemoveEnemyHealthBar();
				}
			}

			bInAttackSphere = false;
			MoveToTarget(main);
		}
		GetWorldTimerManager().ClearTimer(attackTimer);
	}
}



void AEnemy::MoveToTarget(AMainPlayer * target)
{
	SetEnemyState(EEnemyState::ES_MOVE_TO_TARGET);
	if (!(target->PlayerAlive()) || (target->GetHasGameWin()))  return;
	if (enemyController && EnemyAlive() && (!bHurting))
	{
		FAIMoveRequest request;
		request.SetGoalActor(target);
		request.SetAcceptanceRadius(10.0f);
		FNavPathSharedPtr out;
		enemyController->MoveTo(request, &out);
	}
}

void AEnemy::Enemy_Attack()
{

	if (enemyController)
	{
		enemyController->StopMovement();
		SetEnemyState(EEnemyState::ES_ATTACK);
	}
	if (playerTarget)
	{
		if ((playerTarget->PlayerAlive()) && !(playerTarget->GetHasGameWin()))
		{
			if (!bAttacking && EnemyAlive() && bHasValidTarget && (!bHurting))
			{
				bAttacking = true;
				UAnimInstance* animInst = GetMesh()->GetAnimInstance();
				if (animInst && attackMontage)
				{
					animInst->Montage_Play(attackMontage);
					animInst->Montage_JumpToSection(FName("Attack"), attackMontage);
				}
			}
		}

	}

}

void AEnemy::NotifyAnim_AttackEnd()
{
	bAttacking = false;
	if (bInAttackSphere && EnemyAlive() && bHasValidTarget)
	{
		float attackRandTime = FMath::FRandRange(minAttackTime,maxAttackTime);
		GetWorldTimerManager().SetTimer(attackTimer,this,&AEnemy::Enemy_Attack, attackRandTime);
		//Enemy_Attack();
	}
}

void AEnemy::OnAttackOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && EnemyAlive() )
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (!(mainPlayer->PlayerAlive()) || mainPlayer->GetHasGameWin())  return;
			//血液飞溅的粒子特效
			if (mainPlayer->hitParticle)
			{
				const USkeletalMeshSocket* tipSocket = GetMesh()->GetSocketByName("tipSocket");
				UGameplayStatics::SpawnEmitterAtLocation(this, mainPlayer->hitParticle, tipSocket->GetSocketLocation(GetMesh()), FRotator(0.0f), true);
			}
			//打击时的声音
			if (mainPlayer->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, mainPlayer->hitSound);
			}
			if (damType)
			{
				UGameplayStatics::ApplyDamage(mainPlayer, enemyDamage, enemyController,this, damType);
			}
		}
	}
}

void AEnemy::OnAttackOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && EnemyAlive() )
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
		}
	}
}

void AEnemy::NotifyAnim_ActiveEnemyCollison()
{
	attackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::NotifyAnim_DeactiveEnemyCollison()
{
	attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::NotifyAnim_EnemyPlayWing()
{
	if (swingSound)
	{
		UGameplayStatics::PlaySound2D(this, swingSound);
	}
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (health - Damage <= 0.0f)
	{
		health = 0.0f;
		SetEnemyState(EEnemyState::ES_DEAD);
		bHurting =false;
		bAttacking = false;
		Die();
		if (EventInstigator)
		{
			AMainPlayerController* mainController = Cast<AMainPlayerController>(EventInstigator);

			if (mainController)
			{
				//mainController->RemoveEnemyHealthBar();
				AMainPlayer* mainPlayer = Cast<AMainPlayer>(mainController->GetCharacter());
				if (mainPlayer)
				{
					mainPlayer->UpdateEnemyTarget();
				}
			}
		}

	}
	else
	{
		health -= Damage;
		bHurting = true;
		bAttacking = false;
		if (EventInstigator)
		{
			AMainPlayerController* mainController = Cast<AMainPlayerController>(EventInstigator);

			if (mainController)
			{
				//mainController->RemoveEnemyHealthBar();
				AMainPlayer* mainPlayer = Cast<AMainPlayer>(mainController->GetCharacter());
				if (mainPlayer)
				{
					FVector launchDirection = mainPlayer->GetActorForwardVector();
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
		}

	}

	return Damage;
}

void AEnemy::Die()
{
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (animInst && attackMontage)
	{
		animInst->Montage_Play(attackMontage);
		animInst->Montage_JumpToSection(FName("Death"), attackMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	agroSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (deathSound)
	{
		UGameplayStatics::PlaySound2D(this, deathSound);
	}
}

void AEnemy::NotifyAnim_DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(deathTimer,this, &AEnemy::DestroyCall,deathDelay);
}

void AEnemy::DestroyCall()
{
	Destroy();
}

bool AEnemy::EnemyAlive()
{
	return enemyState != EEnemyState::ES_DEAD;
}

void AEnemy::NotifyAnim_HurtingEnd()
{
	bHurting = false;
	if (playerTarget)
	{
		if (playerTarget->PlayerAlive())
		{
			if (bInAttackSphere && EnemyAlive() && bHasValidTarget)
			{
				Enemy_Attack();
			}
		}

	}

}
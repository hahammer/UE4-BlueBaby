// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Pickup.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/MainPlayer.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy/Enemy.h"
#include "Particles/ParticleSystem.h"

AWeapon_Pickup::AWeapon_Pickup()
{
	skeMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeMeshComp"));
	skeMeshComp->SetupAttachment(GetRootComponent());

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComp"));
	widgetComp->SetupAttachment(GetRootComponent());

	attackComp = CreateDefaultSubobject<UBoxComponent>(TEXT("attackComp"));
	attackComp->SetupAttachment(GetRootComponent());

	equipSound = nullptr;

	//Ω«…´π•ª˜µ–»À
	swingSound = nullptr;
	weaponDamage = 10.0f;

	widgetComp->SetVisibility(false);
	widgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

void AWeapon_Pickup::BeginPlay()
{
	Super::BeginPlay();
	attackComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Pickup::OnAttackOverlapBegin);
	attackComp->OnComponentEndOverlap.AddDynamic(this, &AWeapon_Pickup::OnAttackOverlapEnd);

	//…Ë÷√Œ‰∆˜µƒ≈ˆ◊≤
	attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	attackComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	attackComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (weaponHintWidgetAsset)
	{
		widgetComp->SetWidgetClass(weaponHintWidgetAsset);
	}
}



void AWeapon_Pickup::EquipWeapon(AMainPlayer* mainPlayer)
{
	const USkeletalMeshSocket* meshSocket = mainPlayer->GetMesh()->GetSocketByName("RightHandSocket");
	if (meshSocket)
	{
		weaponInstigator = mainPlayer->GetController();
		meshSocket->AttachActor(this, mainPlayer->GetMesh());
		mainPlayer->SetWeaponHold(this);
		mainPlayer->SetPickupOverlap(nullptr);
		bIsRotate = false;
		particleSysComp->Deactivate();
	}
	if (equipSound)
	{
		UGameplayStatics::PlaySound2D(this, equipSound);
	}
	if (weaponHintWidgetAsset)
	{
		widgetComp->SetVisibility(false);
	}
}

void AWeapon_Pickup::OnPickupOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnPickupOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (weaponHintWidgetAsset)
			{
				widgetComp->SetVisibility(true);
			}
			mainPlayer->SetPickupOverlap(this);
		}
	}
}

void AWeapon_Pickup::OnPickupOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (weaponHintWidgetAsset)
			{
				widgetComp->SetVisibility(false);
			}
			mainPlayer->SetPickupOverlap(nullptr);
		}
	}
}

void AWeapon_Pickup::OnAttackOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (enemy)
		{
			//—™“∫∑…Ω¶µƒ¡£◊”Ãÿ–ß
			if (enemy->hitParticle)
			{
				const USkeletalMeshSocket* weaponSocket = skeMeshComp->GetSocketByName("weaponSocket");
				UGameplayStatics::SpawnEmitterAtLocation(this, enemy->hitParticle, weaponSocket->GetSocketLocation(skeMeshComp), FRotator(0.0f), true);
			}
			//¥Úª˜ ±µƒ…˘“Ù
			if (enemy->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, enemy->hitSound);
			}
			//…À∫¶µ–»À
			if (damType)
			{
				UGameplayStatics::ApplyDamage(enemy, weaponDamage, weaponInstigator, this, damType);
			}
		}
	}
}

void AWeapon_Pickup::OnAttackOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (enemy)
		{
		}
	}
}


void AWeapon_Pickup::NotifyAnim_ActiveWeaponCollison()
{
	attackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon_Pickup::NotifyAnim_DeactiveWeaponCollison()
{
	attackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon_Pickup::NotifyAnim_WeaponPlayWing()
{
	if (swingSound)
	{
		UGameplayStatics::PlaySound2D(this, swingSound);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "TimerManager.h"
#include "GladiatorGameCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

// Angle is in degrees
bool IsAngleInCone(const FVector& v1, const FVector& v2, float coneAngle)
{
	FVector nv1 = v1.GetSafeNormal();
	FVector nv2 = v2.GetSafeNormal();

	float angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(nv1, nv2));

	if (FVector::CrossProduct(nv1, nv2).Z <= 0.f)
		angle = -angle;

	if (-coneAngle <= angle && angle <= coneAngle)
		return true;
	else
		return false;
}

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	life = maxLife;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::stopInputs()
{
	canMove = false;
	blockInputs = true;
}

void AEnemy::restartInputs()
{
	canMove = true;
	blockInputs = false;
}

void AEnemy::LifeDown(int value)
{
	if (life > 0)
	{
		life -= value;

		if (life <= 0)
		{
			stopInputs();

			if (DeathAnimation != NULL)
			{
				UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
				if (animInstance != NULL)
				{
					animInstance->Montage_Play(DeathAnimation);
					m_death.Broadcast();
					SetActorEnableCollision(false);
				}
			}
		}
		GetWorldTimerManager().SetTimer(hurtHandle, this, &AEnemy::materialBlink, 0.5f, true, 0.1f);
	}
}

void AEnemy::LifeUp(int value)
{
	if (life < maxLife)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Magenta, FString(TEXT("Healed by value %i"), value));
		life += value;
	}
}

void AEnemy::Attack()
{
	GEngine->AddOnScreenDebugMessage(46, 1.f, FColor::Green, FString(this->GetName() + TEXT(" attacking")));

	if (AttackAnimation != NULL)
	{
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance != NULL)
		{
			animInstance->Montage_Play(AttackAnimation);
		}
	}

	FTimerHandle attackHandle{};
	GetWorldTimerManager().SetTimer(attackHandle, this, &AEnemy::HitEnemies, 0.1f, false);
	AAIController* cont = Cast<AAIController>(GetController());
	cont->GetBlackboardComponent()->SetValueAsEnum("AIState", (uint8)AIState::HOLDING);
	attackFinished = true;
}

void AEnemy::HitEnemies()
{
	if (attackHitbox == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(6486, 1.f, FColor::Magenta, FString(TEXT("Hitbox gone, ") + this->GetName()));
		return;
	}

	attackHitbox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	attackHitbox->UpdateOverlaps();

	TArray<AActor*> overlappingActors;
	attackHitbox->GetOverlappingActors(overlappingActors, AGladiatorGameCharacter::StaticClass());
	for (AActor* actor : overlappingActors)
	{
		AGladiatorGameCharacter* player = Cast<AGladiatorGameCharacter>(actor);
		if (player)
		{
			if (player->blocking)
			{
				FVector playerToEnemy = this->GetActorLocation() - player->GetActorLocation();

				if(IsAngleInCone(player->GetActorForwardVector(), playerToEnemy, 75.f))
					return;
			}

			GEngine->AddOnScreenDebugMessage(646, 1.f, FColor::Green, FString(this->GetName() + TEXT(" attacked ") + player->GetName()));
			player->LifeDown(1);
		}
	}

	GEngine->AddOnScreenDebugMessage(6486, 1.f, FColor::Magenta, FString(TEXT("Hitting enemies")));
	attackHitbox->SetCollisionProfileName(TEXT("NoCollision"));
}

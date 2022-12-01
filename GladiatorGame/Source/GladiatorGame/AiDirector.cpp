// Fill out your copyright notice in the Description page of Project Settings.


#include "AiDirector.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAiDirector::AAiDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAiDirector::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemyList);

	for (AActor* enemy : enemyList)
	{
		enemies.Add(Cast<AEnemy>(enemy));
	}

	//currentAttacker = enemies[0];
	GetWorldTimerManager().SetTimer(attackHandle, this, &AAiDirector::TriggerAttack, attackCooldown, false);
}

// Called every frame
void AAiDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWorldTimerManager().IsTimerActive(attackHandle))
	{
		if (currentAttacker != nullptr && currentAttacker->attackFinished)
			GetWorldTimerManager().SetTimer(attackHandle, this, &AAiDirector::TriggerAttack, attackCooldown, false);
	}
}

AEnemy* AAiDirector::RandomAttacker()
{
	TArray<AEnemy*> enemiesToRemove = {};

	for (AEnemy* entry : enemies)
	{
		if (entry->life <= 0)
		{
			enemiesToRemove.Add(entry);

			if (enemies.Num() == 0)
			{
				return nullptr;
			}
			continue;
		}
	}

	for (AEnemy* enemy : enemiesToRemove)
		enemies.Remove(enemy);

	return enemies[FMath::RandRange(0, enemies.Num() - 1)];
}

void AAiDirector::TriggerAttack()
{
	AEnemy* attacker = RandomAttacker();
	if (attacker == nullptr)
		return;

	AEnemyAIController* controller = Cast<AEnemyAIController> (attacker->GetController());
	if (controller == nullptr)
		return;

	currentAttacker = attacker;
	currentAttacker->attackFinished = false;
	AAIController* cont = Cast<AAIController>(controller);
	cont->GetBlackboardComponent()->SetValueAsEnum("AIState", (uint8)AIState::ATTACKING);
}
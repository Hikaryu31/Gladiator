// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* cont = OwnerComp.GetAIOwner();

	if (AEnemy* enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		enemy->Attack();
	}

	return EBTNodeResult::Type::Succeeded;
}

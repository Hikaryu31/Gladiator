// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Attacking.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_Attacking::UBTDecorator_Attacking(FObjectInitializer const& Object_Initializer)
{
	NodeName = TEXT("Attacking");
}

bool UBTDecorator_Attacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const AEnemy* enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		return enemy->blockInputs;
	}

	return false;
}
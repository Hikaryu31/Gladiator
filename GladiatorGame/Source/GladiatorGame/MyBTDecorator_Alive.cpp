// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_Alive.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTDecorator_Alive::UMyBTDecorator_Alive(FObjectInitializer const& Object_Initializer)
{
	NodeName = TEXT("Alive");
}

bool UMyBTDecorator_Alive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const AEnemy* enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		bool alive = cont->GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
		if (alive)
			return true;
		else
			return false;
	}

	return false;
}
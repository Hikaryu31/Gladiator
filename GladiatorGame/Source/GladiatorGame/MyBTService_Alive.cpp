// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_Alive.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UMyBTService_Alive::UMyBTService_Alive(const FObjectInitializer& Object_Initializer)
{
	NodeName = TEXT("Attack Service");
}

void UMyBTService_Alive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();

	if (const AEnemy* enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		if (enemy->life <= 0)
			cont->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
		else
			cont->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
}
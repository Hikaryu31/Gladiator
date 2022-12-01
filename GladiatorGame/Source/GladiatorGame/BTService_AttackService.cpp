// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AttackService.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_AttackService::UBTService_AttackService(const FObjectInitializer& Object_Initializer)
{
	NodeName = TEXT("Attack Service");
}

void UBTService_AttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();

	if (const AEnemy* enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		APawn* player = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (FVector::DistSquared(player->GetActorLocation(), enemy->GetActorLocation()) < (200.f * 200.f))
		{
			FVector direction = player->GetActorLocation() - enemy->GetActorLocation();
			direction.Normalize();
			FVector oppositeDirection = direction * -1.f;
			cont->MoveToLocation(enemy->GetActorLocation() + oppositeDirection * 200.f, 5.f);
		}
	}
}
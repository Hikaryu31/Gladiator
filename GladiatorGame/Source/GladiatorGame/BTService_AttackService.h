// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_AttackService.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTService_AttackService : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_AttackService(const FObjectInitializer& Object_Initializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

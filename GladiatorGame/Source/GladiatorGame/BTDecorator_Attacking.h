// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_Attacking.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTDecorator_Attacking : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_Attacking(FObjectInitializer const& Object_Initializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

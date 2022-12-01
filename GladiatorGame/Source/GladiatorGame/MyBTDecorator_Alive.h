// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "MyBTDecorator_Alive.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UMyBTDecorator_Alive : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMyBTDecorator_Alive(FObjectInitializer const& Object_Initializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_Alive.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UMyBTService_Alive : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMyBTService_Alive(const FObjectInitializer& Object_Initializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

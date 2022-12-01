// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UENUM(BlueprintType)
enum class AIState : uint8
{
	HOLDING UMETA(DisplayName = "Holding"),
	ATTACKING UMETA(DisplayName = "Attacking"),
	RECOVERY UMETA(DisplayName = "Recovery"),
};

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController(const FObjectInitializer& Object_Initializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnPossess(APawn* const pawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTreeComponent* behaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTree* behaviorTree;

	class UBlackboardComponent* blackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
		AEnemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		TEnumAsByte<AIState> AIstate;
};

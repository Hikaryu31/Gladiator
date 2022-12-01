// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& Object_Initializer) : AAIController(Object_Initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("/Game/AI/EnemyBT.EnemyBT"));

	if (obj.Succeeded())
	{
		behaviorTree = obj.Object;
	}
	behaviorTreeComponent = Object_Initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("behaviorTreeComponent"));
	blackboardComponent = Object_Initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("blackboardComponent"));

	SetActorTickEnabled(true);
	bAllowStrafe = true;


}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(behaviorTree);
	behaviorTreeComponent->StartTree(*behaviorTree);
	blackboardComponent->SetValueAsObject("TargetActor", UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	blackboardComponent->SetValueAsBool("Alive", true);
}

void AEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (me == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-0, 1.f, FColor::Red, FString(TEXT("nullptr")));
		return;
	}

	bool alive = blackboardComponent->GetValueAsBool("Alive");
	if (!alive)
		UnPossess();
}

void AEnemyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	me = Cast<AEnemy>(GetPawn());
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

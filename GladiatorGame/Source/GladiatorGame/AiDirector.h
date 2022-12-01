// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AiDirector.generated.h"

UCLASS()
class GLADIATORGAME_API AAiDirector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAiDirector();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemies)
		TArray<AActor*> enemyList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemies)
		TArray<AEnemy*> enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemies)
		float attackCooldown = 3.f;

	FTimerHandle attackHandle{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void TriggerAttack();
	AEnemy* RandomAttacker();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AEnemy* currentAttacker;

};

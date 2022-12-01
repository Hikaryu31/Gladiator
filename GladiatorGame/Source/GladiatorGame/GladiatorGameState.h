// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GladiatorGameState.generated.h"

class AEnemy;

// Tells GameMode when game ended, bool = did player win?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelEnd, bool, hasWon);

UCLASS()
class GLADIATORGAME_API AGladiatorGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGladiatorGameState();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReasonType) override;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
		FLevelEnd m_endLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemies")
		TArray<AEnemy*> enemies;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, Category = "Enemies")
		int32 enemiesAlive;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, Category = "Enemies")
		class AGladiatorGameCharacter* player;

	UFUNCTION()
	void testEndLevel(bool test);

	UFUNCTION()
	void OnEnemyKilled();

	UFUNCTION()
	void OnPlayerDeath();
};

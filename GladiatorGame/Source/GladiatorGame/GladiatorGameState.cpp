// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorGameState.h"
#include "Enemy.h"
#include "Containers/Array.h"
#include "GladiatorGameCharacter.h"
#include "Kismet/GameplayStatics.h"


AGladiatorGameState::AGladiatorGameState()
{

	FMessageLog("MessageLog").Message(EMessageSeverity::PerformanceWarning, FText::FromString(TEXT("State constructor called")));
}

void AGladiatorGameState::BeginPlay()
{
	Super::BeginPlay();
	AActor* playerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGladiatorGameCharacter::StaticClass());
	player = Cast<AGladiatorGameCharacter>(playerActor);
	FMessageLog("MessageLog").Message(EMessageSeverity::PerformanceWarning, FText::FromString(TEXT("State begin play called")));

	if (player == nullptr)
	{
		FMessageLog("MessageLog").Message(EMessageSeverity::Info, FText::FromString(TEXT("Player null")));
		return;
	}

	player->m_death.AddDynamic(this, &AGladiatorGameState::OnPlayerDeath);
	FMessageLog("MessageLog").Message(EMessageSeverity::PerformanceWarning, FText::FromString(TEXT("Player death linked")));


	TArray<AActor*> enemiesAsActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemiesAsActors);

	for (AActor* actor : enemiesAsActors)
	{
		enemies.Add(Cast<AEnemy>(actor));
		enemies[enemies.Num() - 1]->m_death.AddDynamic(this, &AGladiatorGameState::OnEnemyKilled);
	}
	enemiesAlive = enemies.Num();
	FMessageLog("MessageLog").Message(EMessageSeverity::PerformanceWarning, FText::FromString(TEXT("Enemies death linked")));


	m_endLevel.AddDynamic(this, &AGladiatorGameState::testEndLevel);
	FMessageLog("MessageLog").Message(EMessageSeverity::PerformanceWarning, FText::FromString(TEXT("End level linked")));

}

void AGladiatorGameState::EndPlay(const EEndPlayReason::Type EndPlayReasonType)
{
	Super::EndPlay(EndPlayReasonType);
	FMessageLog("MessageLog").Message(EMessageSeverity::PerformanceWarning, FText::FromString(TEXT("State end play called")));

	player->m_death.RemoveDynamic(this, &AGladiatorGameState::OnPlayerDeath);

	for (AEnemy* enemy : enemies)
	{
		enemy->m_death.RemoveDynamic(this, &AGladiatorGameState::OnEnemyKilled);
	}

	m_endLevel.RemoveDynamic(this, &AGladiatorGameState::testEndLevel);
}

void AGladiatorGameState::testEndLevel(bool test)
{
	GEngine->AddOnScreenDebugMessage(784, 7.2f, FColor::Turquoise, FString(TEXT("EndLevel delegate, bool = ")));
	FMessageLog("MessageLog").Message(EMessageSeverity::Info, FText::FromString(TEXT("End level")));
}

void AGladiatorGameState::OnPlayerDeath()
{
	GEngine->AddOnScreenDebugMessage(7844, 2.2f, FColor::Red, FString(TEXT("Player died ")));
	FMessageLog("MessageLog").Warning(FText::FromString(TEXT("Player Died")));
	m_endLevel.Broadcast(false);
}

void AGladiatorGameState::OnEnemyKilled()
{
	GEngine->AddOnScreenDebugMessage(7844, 2.2f, FColor::Red, FString(TEXT("Enemy died ")));
	FMessageLog("MessageLog").Message(EMessageSeverity::Info, FText::FromString(TEXT("Enemy Died")));

	enemiesAlive--;

	if (enemiesAlive <= 0)
	{
		m_endLevel.Broadcast(true);
	}
}
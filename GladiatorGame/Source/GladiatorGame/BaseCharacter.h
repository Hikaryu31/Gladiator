// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);

UCLASS()
class GLADIATORGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* DeathAnimation;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
		FDeath m_death;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		int life;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		int maxLife = 5;

	bool canMove = true;
	bool blockInputs = false;
	FTimerHandle hurtHandle{};

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		virtual void stopInputs() {};
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		virtual void restartInputs() {};

protected:
	USkeletalMeshComponent* weaponComponent;
	USkeletalMeshComponent* shieldComponent;

	UPROPERTY(VisibleAnywhere, Category = "Attack hitbox")
		class UBoxComponent* attackHitbox;

	int callsRemaining = 2;
	void materialBlink();
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GladiatorGameCharacter.generated.h"

// Create event handlers
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBackdash, bool, showBackdashProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthUpdate);

UCLASS(config = Game)
class AGladiatorGameCharacter : public ABaseCharacter
{
	GENERATED_BODY()

// PRIVATE VARIABLES
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


// CONSTRUCTORS, UPDATE FUNCTIONS, PUBLIC VARIABLES
public:
	AGladiatorGameCharacter();

	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
		FHealthUpdate m_healthUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
		FBackdash m_backdash;

// PUBLIC FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "Delegate|Debug")
		void DebugLifeDown();
	UFUNCTION(BlueprintCallable, Category = "Delegate|Debug")
		void DebugLifeUp();

	virtual void stopInputs() override;
	virtual void restartInputs() override;

	void LifeUp(int value = 1);
	void LifeDown(int value = 1);
	
protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Attack();
	void StartBlocking();
	void StopBlocking();

	void Backdash();

	void HitEnemies();

	UFUNCTION()
	void printDelegate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float bdashTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxBackdashTimer = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float backdashStrength = 10.f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	bool wasBlocking = false;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	bool blocking = false;
};


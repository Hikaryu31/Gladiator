// Copyright Epic Games, Inc. All Rights Reserved.

#include "GladiatorGameCharacter.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//////////////////////////////////////////////////////////////////////////
// AGladiatorGameCharacter

AGladiatorGameCharacter::AGladiatorGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	life = maxLife;

	//m_healthUpdate.AddDynamic(this, &AGladiatorGameCharacter::printDelegate);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGladiatorGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGladiatorGameCharacter::Attack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AGladiatorGameCharacter::StartBlocking);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AGladiatorGameCharacter::StopBlocking);

	PlayerInputComponent->BindAction("Backdash", IE_Pressed, this, &AGladiatorGameCharacter::Backdash);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGladiatorGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGladiatorGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGladiatorGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGladiatorGameCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("debugDamagePlayer", IE_Pressed, this, &AGladiatorGameCharacter::DebugLifeDown);
	PlayerInputComponent->BindAction("debugHealPlayer", IE_Pressed, this, &AGladiatorGameCharacter::DebugLifeUp);
}

void AGladiatorGameCharacter::Backdash()
{
	if (bdashTimer <= 0)
	{
		LaunchCharacter(-GetActorForwardVector() * backdashStrength, true, false);
		bdashTimer = maxBackdashTimer;
		m_backdash.Broadcast(true);
	}
}

void AGladiatorGameCharacter::Attack()
{
	if (!blockInputs)
	{
		if (blocking)
		{
			StopBlocking();
			wasBlocking = true;
		}

		if (AttackAnimation != NULL)
		{
			UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
			if (animInstance != NULL)
			{
				animInstance->Montage_Play(AttackAnimation);
			}
		}
		FTimerHandle attackHandle{};
		GetWorldTimerManager().SetTimer(attackHandle, this, &AGladiatorGameCharacter::HitEnemies, 0.1f, false);
	}
}

void AGladiatorGameCharacter::HitEnemies()
{
	if (attackHitbox == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(6486, 1.f, FColor::Magenta, FString(TEXT("Hitbox gone")));
		return;
	}

	attackHitbox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	attackHitbox->UpdateOverlaps();
	
	TArray<AActor*> overlappingActors;
	attackHitbox->GetOverlappingActors(overlappingActors, AEnemy::StaticClass());
	for (AActor* actor : overlappingActors)
	{
		AEnemy* enemy = Cast<AEnemy>(actor);
		if (enemy)
		{
			GEngine->AddOnScreenDebugMessage(646, 1.f, FColor::Green, FString(TEXT("Enemy detected") + enemy->GetName()));
			enemy->LifeDown(1);
		}
	}

	GEngine->AddOnScreenDebugMessage(6486, 1.f, FColor::Magenta, FString(TEXT("Hitting enemies")));
	attackHitbox->SetCollisionProfileName(TEXT("NoCollision"));
}

void AGladiatorGameCharacter::StartBlocking()
{
	if (!blockInputs)
	{
		if (!blocking)
		{
			blocking = true;
			FVector scale = shieldComponent->GetRelativeScale3D();
			shieldComponent->SetRelativeScale3D(scale * 3);
			canMove = false;
		}
	}
}

void AGladiatorGameCharacter::StopBlocking()
{
	if (!blockInputs)
	{
		if (blocking)
		{
			blocking = false;
			FVector scale = shieldComponent->GetRelativeScale3D();
			shieldComponent->SetRelativeScale3D(scale / 3);
			canMove = true;
		}
	}
	wasBlocking = false;
}

void AGladiatorGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGladiatorGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGladiatorGameCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && canMove)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGladiatorGameCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) && canMove)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGladiatorGameCharacter::DebugLifeDown()
{
	LifeDown(1);
}

void AGladiatorGameCharacter::DebugLifeUp()
{
	LifeUp(1);
}

void AGladiatorGameCharacter::stopInputs()
{
	canMove = false;
	if (blocking)
	{
		StopBlocking();
		wasBlocking = true;
	}
	blockInputs = true;
}

void AGladiatorGameCharacter::restartInputs()
{
	canMove = true;
	blockInputs = false;
	if (wasBlocking)
	{
		StartBlocking();
		wasBlocking = false;
	}
}

void AGladiatorGameCharacter::LifeDown(int value)
{
	if (life > 0)
	{
		life -= value;
		m_healthUpdate.Broadcast();

		if (life <= 0)
		{
			stopInputs();

			if (DeathAnimation != NULL)
			{
				UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
				if (animInstance != NULL)
				{
					animInstance->Montage_Play(DeathAnimation);
					m_death.Broadcast();
				}
			}
		}
			GetWorldTimerManager().SetTimer(hurtHandle, this, &AGladiatorGameCharacter::materialBlink, 0.5f, true, 0.1f);
	}
}

void AGladiatorGameCharacter::LifeUp(int value)
{
	if (life < maxLife)
	{
		life += value;
		m_healthUpdate.Broadcast();
	}
}

void AGladiatorGameCharacter::printDelegate()
{
	GEngine->AddOnScreenDebugMessage(-0, 1.f, FColor::Emerald, FString(TEXT("delegate successful")));
}

void AGladiatorGameCharacter::Tick(float DeltaTime)
{
	if (bdashTimer > 0.f)
	{
		bdashTimer -= DeltaTime;

		if (bdashTimer <= 0.f)
			m_backdash.Broadcast(false);
	}
}

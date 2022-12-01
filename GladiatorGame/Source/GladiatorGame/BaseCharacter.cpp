
#include "BaseCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABaseCharacter

ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 65.0f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AttackAnimation = CreateDefaultSubobject<UAnimMontage>(TEXT("/Game/Characters/DwarfGrunt/Anims/Attack_1_Montage.Attack_1_Montage"));
	DeathAnimation = CreateDefaultSubobject<UAnimMontage>(TEXT("/Game/Characters/DwarfGrunt/Anims/Death2_Montage.Death2_Montage"));

	weaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> weapon(TEXT("/Game/Characters/DwarfGrunt/SkelMesh/WarHammer.WarHammer"));

	if (weapon.Succeeded())
	{
		weaponComponent->SetSkeletalMesh(weapon.Object);
		weaponComponent->SetupAttachment(GetMesh(), TEXT("WeaponPoint"));
	}

	shieldComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> shield(TEXT("/Game/Characters/DwarfGrunt/SkelMesh/Shield_Skel.Shield_Skel"));

	if (shield.Succeeded())
	{
		shieldComponent->SetSkeletalMesh(shield.Object);
		shieldComponent->SetupAttachment(GetMesh(), TEXT("DualWeaponPoint"));
	}

	attackHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack hitbox"));
	attackHitbox->bEditableWhenInherited = true;
	attackHitbox->SetupAttachment(GetCapsuleComponent());
	attackHitbox->SetRelativeLocation(FVector(55.f, 0.f, 0.f));
	attackHitbox->SetRelativeScale3D(FVector(1.25f, 1.f, 1.25f));
	attackHitbox->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABaseCharacter::materialBlink()
{
	if (callsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(hurtHandle);
		callsRemaining = 2;
		GetMesh()->SetScalarParameterValueOnMaterials("Activate", 0.f);
		return;
	}

	GetMesh()->SetScalarParameterValueOnMaterials("Activate", 1.f);
	callsRemaining--;
}
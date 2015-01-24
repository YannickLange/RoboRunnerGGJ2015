// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#include "RoboRunners.h"
#include "Monster.h"
#include "Robot.h"

AMonster::AMonster(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StunValue = 5;
	StartStunValue = 5;
	Health = 100;
	MaxWalkSpeed = 250;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 20.f;
	BaseLookUpRate = 20.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EMonsterState PrevState = MonsterState;

	switch (HittingRobots.Num())
	{
	case 3:
		MonsterState = EMonsterState::DAMAGE;
		Health -= DeltaSeconds * 4;
		StunValue = 4.0f;
		break;
	case 2:
		MonsterState = EMonsterState::STUN;
		StunValue = 3.0f;
		break;
	case 1:
		MonsterState = EMonsterState::STUN;
		StunValue = 1.5f;
		break;
	case 0:
		MonsterState = EMonsterState::NONE;
		StunValue = StartStunValue;
		break;
	}

	switch (MonsterState)
	{
	case EMonsterState::DAMAGE:
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed / StunValue;
		break;
	case EMonsterState::GAIN:

		break;
	case EMonsterState::STUN:
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed / StunValue;
		break;
	case EMonsterState::NONE:
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		break;
	}
}

void AMonster::LaserHit(ARobot* RobotActor, const FHitResult& HitResult)
{
	if (RobotActor->ElementColor == ElementColor && HittingRobots.Find(RobotActor))
	{
		HittingRobots.Add(RobotActor);
	}
}

void AMonster::RemoveLaserHit(ARobot* RobotActor)
{
	HittingRobots.RemoveSingle(RobotActor);
}

void AMonster::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Bash", EInputEvent::IE_Pressed, this, &AMonster::Bash);
	InputComponent->BindAxis("MoveForward", this, &AMonster::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMonster::MoveRight);
}

void AMonster::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMonster::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
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

void AMonster::Bash()
{
	// find out which way is forward
	const FRotator YawRotation(0, GetActorRotation().Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	FVector BashVector = YawRotation.Vector() * 10;
	LaunchCharacter(BashVector, true, true);
	UE_LOG(GGJ, Log, TEXT("BASH!!!"));
}
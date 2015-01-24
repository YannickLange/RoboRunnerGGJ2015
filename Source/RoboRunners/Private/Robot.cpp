// Fill out your copyright notice in the Description page of Project Settings.

#include "RoboRunners.h"
#include "Robot.h"
#include "Monster.h"

ARobot::ARobot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

	AimDistance = 4000.0f;
	AimStartDistance = 150.0f;
	bIsHittingMonster = false;
}

void ARobot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickLaser(DeltaSeconds);
}

void ARobot::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARobot::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARobot::MoveRight);
	InputComponent->BindAxis("AimX", this, &ARobot::AimX);
	InputComponent->BindAxis("AimY", this, &ARobot::AimY);
}


void ARobot::MoveForward(float Value)
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

void ARobot::MoveRight(float Value)
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

void ARobot::AimX(float Value)
{
	AimLocation.X = Value;
}

void ARobot::AimY(float Value)
{
	AimLocation.Y = Value;
}

void ARobot::TickLaser(float DeltaSeconds)
{
	if (AimLocation.X == 0 && AimLocation.Y == 0)
	{
		return;
	}

	FVector AimWorldLocation = GetActorLocation();
	AimWorldLocation.Y -= AimLocation.X;
	AimWorldLocation.X += AimLocation.Y;

	FVector Direction = GetActorLocation() - AimWorldLocation;
	Direction.Normalize();

	AimEndLocation = AimWorldLocation + AimDistance * Direction;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	FCollisionObjectQueryParams CollisionObjectParams;
	if (GetWorld()->LineTraceSingle(HitResult, AimWorldLocation + (AimStartDistance * Direction), AimEndLocation, CollisionParams, CollisionObjectParams))
	{
		if (HitResult.GetActor())
		{
			if (HitResult.GetActor()->GetClass()->IsChildOf(AMonster::StaticClass()) && !bIsHittingMonster)
			{
				//Spawn particle
				Cast<AMonster>(HitResult.GetActor())->LaserHit(this, HitResult);
				bIsHittingMonster = true;
			}
			else
			{
				bIsHittingMonster = false;
				Cast<AMonster>(HitResult.GetActor())->RemoveLaserHit(this);
			}
		}
		
		// Hit
		AimEndLocation = HitResult.ImpactPoint;
	}
}


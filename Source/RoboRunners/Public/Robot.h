// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Robot.generated.h"

/**
 * 
 */
UCLASS()
class ROBORUNNERS_API ARobot : public ACharacter
{
	GENERATED_BODY()

		virtual void Tick(float DeltaSeconds);
	virtual void BeginPlay();
public:
	ARobot(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Laser */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RobotAim)
		FVector AimEndLocation;

	/** Laser */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RobotAim)
		float AimDistance;

	/** Laser */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobotAim)
		FVector AimLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RobotAim)
		float AimStartDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ElementColor)
		FColor ElementColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobotAim)
	bool bIsShooting;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RobotAim)
	float OutOfViewTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobotAim)
	float OutOfViewStartTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobotAim)
	class AMonster* Monster;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobotAim)
	bool bIsHittingMonster;
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void AimX(float Value);
	void AimY(float Value);
	void ShootStart();
	void ShootStop();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	
	
private:
	void TickLaser(float DeltaSeconds);
	//void TickOutOfView(float DeltaSeconds);
	//void Respawn();

	FVector PrevLocation;
	
};
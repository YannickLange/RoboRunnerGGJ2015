// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMonsterState : uint8
{
	DAMAGE 	UMETA(DisplayName = "DAMAGE"),
	STUN 	UMETA(DisplayName = "STUN"),
	GAIN	UMETA(DisplayName = "GAIN"),
	NONE 	UMETA(DisplayName = "NONE")
};


UCLASS(config = Game)
class AMonster : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	virtual void Tick(float DeltaSeconds);
public:
	AMonster(const FObjectInitializer& ObjectInitializer);

	void LaserHit(class ARobot* RobotActor, const FHitResult& HitResult);
	void RemoveLaserHit(class ARobot* RobotActor);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ElementColor)
	FColor ElementColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Robots)
	TArray<class ARobot*> HittingRobots;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Robots)
	TArray<class ARobot*> GainRobots;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MYCat)
	float StunValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MYCat)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EMonsterState MonsterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MYCat)
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MYCat)
	float BashTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MYCat)
	float SpawnRadius;
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Bash();
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	float StartStunValue;
	float BashStartTime;
};


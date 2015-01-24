// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "RoboRunnersGameMode.generated.h"

UCLASS(minimalapi)
class ARoboRunnersGameMode : public AGameMode
{
	GENERATED_BODY()



	virtual void BeginPlay();
public:
	ARoboRunnersGameMode(const FObjectInitializer& ObjectInitializer);	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RobotPawn)
	TSubclassOf<APawn> RobotCharacter;
};




#include "RoboRunners.h"
#include "RoboRunnersGameMode.h"
#include "Monster.h"
#include "Robot.h"

ARoboRunnersGameMode::ARoboRunnersGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Monster"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ARobot> RobotPawnBPClass(TEXT("/Game/Blueprints/Robot"));
	RobotCharacter = RobotPawnBPClass.Class;
}

void ARoboRunnersGameMode::BeginPlay()
{
	FString OutError;
	UGameViewportClient* Viewport = GetWorld()->GetGameViewport();

	DefaultPawnClass = RobotCharacter;
	Viewport->CreatePlayer(1, OutError, true);
	Viewport->CreatePlayer(2, OutError, true);
	Viewport->CreatePlayer(3, OutError, true);
}

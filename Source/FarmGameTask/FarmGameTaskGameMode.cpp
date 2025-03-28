// Copyright Epic Games, Inc. All Rights Reserved.

#include "FarmGameTaskGameMode.h"
#include "FarmGameTaskCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"

AFarmGameTaskGameMode::AFarmGameTaskGameMode()
{
	GameStateClass = AFarmGameTaskGameState::StaticClass();
	PlayerControllerClass = AFarmGameTaskPlayerController::StaticClass();
	PlayerStateClass = AFarmGameTaskPlayerState::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AFarmGameTaskGameMode::StartPlay()
{
	Super::StartPlay();
}

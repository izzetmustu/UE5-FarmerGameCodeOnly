// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FarmGameTaskGameMode.generated.h"

UCLASS(minimalapi)
class AFarmGameTaskGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFarmGameTaskGameMode();
protected:
	virtual void StartPlay() override;
};




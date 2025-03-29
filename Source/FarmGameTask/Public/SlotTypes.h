#pragma once

#include "CoreMinimal.h"
#include "SlotTypes.generated.h"

UENUM(BlueprintType)
enum class ESlotState : uint8
{
	Empty UMETA(DisplayName="Empty"),
	Growing UMETA(DisplayName="Growing"),
	ReadyToHarvest UMETA(DisplayName="ReadyToHarvest"),
	Damaged UMETA(DisplayName="Damaged"),
	MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class ESlotSeedType : uint8
{
	None UMETA(DisplayName="None"),
	Wheat UMETA(DisplayName="Wheat"),
	Corn UMETA(DisplayName="Corn"),
	MAX UMETA(DisplayName="DefaultMAX")
};

USTRUCT(BlueprintType)
struct FSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlotState State;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlotSeedType SeedType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimeRemaining;
	FSlotInfo()
	{
		State = ESlotState::Empty;
		SeedType = ESlotSeedType::None;
		TimeRemaining = 0.0f;
	}
	FSlotInfo(ESlotState inState, ESlotSeedType inSeedType, float inTimeRemaining)
	{
		State = inState;
		SeedType = inSeedType;
		TimeRemaining = inTimeRemaining;
	}
};
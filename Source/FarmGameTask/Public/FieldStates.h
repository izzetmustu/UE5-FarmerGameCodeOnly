#pragma once

UENUM(BlueprintType)
enum class EFieldState : uint8
{
	Empty UMETA(DisplayName="Empty"),
	Planted UMETA(DisplayName="Planted"),
	Growing UMETA(DisplayName="Growing"),
	ReadyToHarvest UMETA(DisplayName="ReadyToHarvest"),
	Damaged UMETA(DisplayName="Damaged")
};
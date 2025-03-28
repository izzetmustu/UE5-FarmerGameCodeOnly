#pragma once

UENUM(BlueprintType)
enum class ECropType  : uint8
{
	Wheat UMETA(DisplayName="Wheat"),
	Corn UMETA(DisplayName="Corn"),
	ProcessedWheat UMETA(DisplayName="ProcessedWheat"),
	ProcessedCorn UMETA(DisplayName="ProcessedCorn"),
	MAX UMETA(DisplayName="DefaultMAX")
};

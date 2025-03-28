#pragma once

UENUM(BlueprintType)
enum class ECropType  : uint8
{
	Wheat UMETA(DisplayName="Wheat"),
	Corn UMETA(DisplayName="Corn"),
	HarvestedWheat UMETA(DisplayName="HarvestedWheat"),
	HarvestedCorn UMETA(DisplayName="HarvestedCorn"),
	Damaged UMETA(DisplayName="Damaged"),
	MAX UMETA(DisplayName="DefaultMAX")
};

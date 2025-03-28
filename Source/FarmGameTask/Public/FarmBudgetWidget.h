// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FarmBudgetWidget.generated.h"

class UTextBlock;

UCLASS()
class FARMGAMETASK_API UFarmBudgetWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateFarmBudgetDisplay();
protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BudgetTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneySpent;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CropsHarvested;
};

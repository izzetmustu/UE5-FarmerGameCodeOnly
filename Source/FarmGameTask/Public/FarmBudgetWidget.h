// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FarmBudgetWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class FARMGAMETASK_API UFarmBudgetWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/** 
	 * NativeConstruct is called once the underlying Slate widget is fully created.
	 * It's a good place to do initial setup (e.g., caching references, binding delegates).
	 */
	virtual void NativeConstruct() override;

	/**
	 * Update the on-screen text to show the latest Farm Budget.
	 * Call this whenever the Budget changes (or on a timer).
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateFarmBudgetDisplay();
protected:

	/**
	 * A reference to the TextBlock in your Widget Blueprint where you'll display the budget.
	 * Bind this in your Blueprint subclass (e.g. by creating a "TextBlock_Amount" and exposing it as a variable).
	 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BudgetTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CropsHarvested;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneySpent;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FarmScore;
};

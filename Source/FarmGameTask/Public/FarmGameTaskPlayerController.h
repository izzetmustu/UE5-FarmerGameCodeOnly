// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FarmGameTaskPlayerController.generated.h"

class UFarmBudgetWidget;
class USalesCounterWidget;
class AFarmGameTaskGameState;
class UInputAction;
class UInputMappingContext;
class ASalesCounter;

/**
 * 
 */
UCLASS()
class FARMGAMETASK_API AFarmGameTaskPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AFarmGameTaskPlayerController();

	UFUNCTION(BlueprintCallable, Category="Farm")
	void BuySeeds(int32 SeedCost);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UFarmBudgetWidget> FarmBudgetWidgetClass;
	UPROPERTY()
	UFarmBudgetWidget* FarmBudgetWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<USalesCounterWidget> SalesCounterWidgetClass;
	UPROPERTY()
	USalesCounterWidget* SalesCounterWidgetInstance;
	
	UPROPERTY(BlueprintReadOnly, Category="UI")
	bool bIsSalesWidgetVisible;
	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateSalesWidget();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowSalesCounter(ASalesCounter* InSalesCounter);
	UFUNCTION(BlueprintCallable, Category="UI")
	void HideSalesCounter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Interact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* IMC_MyMappings;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttemptBuyCrop(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttemptPlaceCrop(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount);

protected:
	virtual void BeginPlay() override;
	
	void Interact();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();
	
	virtual void SetupInputComponent() override;
};

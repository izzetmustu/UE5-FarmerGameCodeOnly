// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlotTypes.h"
#include "FarmGameTaskPlayerController.generated.h"

class UFarmBudgetWidget;
class USalesCounterWidget;
class UInteractSeedWidget;
class AFarmGameTaskGameState;
class UInputAction;
class UInputMappingContext;
class ASalesCounter;
class ACropSlot;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UInteractSeedWidget> InteractSeedWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UInteractSeedWidget* InteractSeedWidgetInstance;
	
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
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteractWithSlot(ACropSlot* TargetSlot, FSlotInfo Info);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttemptSowSlot(ACropSlot* TargetSlot, FSlotInfo Info);
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void Interact();
	
	virtual void SetupInputComponent() override;
};

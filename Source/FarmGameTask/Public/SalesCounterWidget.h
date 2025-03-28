// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SalesCounterWidget.generated.h"

class UTextBlock;
class UButton;
class ASalesCounter;

UCLASS()
class FARMGAMETASK_API USalesCounterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Sales")
    void SetSalesCounter(ASalesCounter* InSalesCounter);
    UFUNCTION(BlueprintCallable, Category="Sales")
    void UpdateCounterDisplay();
protected:
    virtual void NativeConstruct() override;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* BudgetTextBlock;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* WheatCountText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CornCountText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HarvestedWheatCountText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HarvestedCornCountText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WheatOwned;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CornOwned;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HarvestedWheatOwned;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HarvestedCornOwned;
    
    UPROPERTY(meta = (BindWidget))
    UButton* BuyWheatButton;
    UPROPERTY(meta = (BindWidget))
    UButton* BuyCornButton;
    UPROPERTY(meta = (BindWidget))
    UButton* PlaceWheatButton;
    UPROPERTY(meta = (BindWidget))
    UButton* PlaceCornButton;
    
    UPROPERTY()
    ASalesCounter* LinkedSalesCounter;

    // Button callbacks
    UFUNCTION()
    void OnBuyWheatClicked();
    UFUNCTION()
    void OnBuyCornClicked();
    UFUNCTION()
    void OnPlaceWheatClicked();
    UFUNCTION()
    void OnPlaceCornClicked();

};

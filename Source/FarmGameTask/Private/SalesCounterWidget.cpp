// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmGameTask/Public/SalesCounterWidget.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "FarmGameTask//Public/CropTypes.h"
#include "FarmGameTaskGameState.h"
#include "FarmGameTaskPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "FarmGameTask/Public/SalesCounter.h"
#include "FarmGameTask/Public/CropTypes.h"

void USalesCounterWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events (if the button pointers are valid)
    if (BuyWheatButton)
    {
        BuyWheatButton->OnClicked.AddDynamic(this, &USalesCounterWidget::OnBuyWheatClicked);
    }
    if (BuyCornButton)
    {
        BuyCornButton->OnClicked.AddDynamic(this, &USalesCounterWidget::OnBuyCornClicked);
    }
    if (PlaceWheatButton)
    {
        PlaceWheatButton->OnClicked.AddDynamic(this, &USalesCounterWidget::OnPlaceWheatClicked);
    }
    if (PlaceCornButton)
    {
        PlaceCornButton->OnClicked.AddDynamic(this, &USalesCounterWidget::OnPlaceCornClicked);
    }

    UpdateCounterDisplay();
}

void USalesCounterWidget::SetSalesCounter(ASalesCounter* InSalesCounter)
{
    LinkedSalesCounter = InSalesCounter;

    // Immediately update the display
    UpdateCounterDisplay();
}

void USalesCounterWidget::UpdateCounterDisplay()
{
    if (!LinkedSalesCounter)
    {
        if (BudgetTextBlock) BudgetTextBlock->SetText(FText::FromString("0"));
        if (WheatCountText) WheatCountText->SetText(FText::FromString("0"));
        if (CornCountText)  CornCountText->SetText(FText::FromString("0"));
        if (ProcessedWheatCountText) ProcessedWheatCountText->SetText(FText::FromString("0"));
        if (ProcessedCornCountText) ProcessedCornCountText->SetText(FText::FromString("0"));
        if (WheatOwned) WheatOwned->SetText(FText::FromString("0"));
        if (CornOwned) CornOwned->SetText(FText::FromString("0"));
        if (ProcessedWheatOwned) ProcessedWheatOwned->SetText(FText::FromString("0"));
        if (ProcessedCornOwned) ProcessedCornOwned->SetText(FText::FromString("0"));
        return;
    }
    
    const int32 CurrentBudget = LinkedSalesCounter->GetFarmGameState()->GetFarmBudget();
    const int32 CurrentWheat = LinkedSalesCounter->GetCropCount(ECropType::Wheat);
    const int32 CurrentCorn  = LinkedSalesCounter->GetCropCount(ECropType::Corn);
    const int32 CurrentProcessedWheat = LinkedSalesCounter->GetProcessedWheatCount();
    const int32 CurrentProcessedCorn  = LinkedSalesCounter->GetProcessedCornCount();
    

    BudgetTextBlock->SetText(FText::AsNumber(CurrentBudget));
    WheatCountText->SetText(FText::AsNumber(CurrentWheat));
    CornCountText->SetText(FText::AsNumber(CurrentCorn));
    ProcessedWheatCountText->SetText(FText::AsNumber(CurrentProcessedWheat));
    ProcessedCornCountText->SetText(FText::AsNumber(CurrentProcessedCorn));

    // Inventory
    WheatOwned->SetText(FText::AsNumber(Cast<AFarmGameTaskPlayerState>(GetOwningPlayerState())->GetWheatCount()));
    CornOwned->SetText(FText::AsNumber(Cast<AFarmGameTaskPlayerState>(GetOwningPlayerState())->GetCornCount()));
    ProcessedWheatOwned->SetText(FText::AsNumber(Cast<AFarmGameTaskPlayerState>(GetOwningPlayerState())->GetProcessedWheatCount()));
    ProcessedCornOwned->SetText(FText::AsNumber(Cast<AFarmGameTaskPlayerState>(GetOwningPlayerState())->GetProcessedCornCount()));
}

void USalesCounterWidget::OnBuyWheatClicked()
{
    if (!LinkedSalesCounter) return;
    if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetOwningPlayer()))
    {
        PC->ServerAttemptBuyCrop(LinkedSalesCounter, ECropType::Wheat, 1);
    }
}

void USalesCounterWidget::OnBuyCornClicked()
{
    if (!LinkedSalesCounter) return;

    if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetOwningPlayer()))
    {
        PC->ServerAttemptBuyCrop(LinkedSalesCounter, ECropType::Corn, 1);
    }
}

void USalesCounterWidget::OnPlaceWheatClicked()
{
    if (!LinkedSalesCounter) return;

    if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetOwningPlayer()))
    {
        PC->ServerAttemptPlaceCrop(LinkedSalesCounter, ECropType::ProcessedWheat, 1);
    }
}

void USalesCounterWidget::OnPlaceCornClicked()
{
    if (!LinkedSalesCounter) return;

    if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetOwningPlayer()))
    {
        PC->ServerAttemptPlaceCrop(LinkedSalesCounter, ECropType::ProcessedCorn, 1);
    }
}
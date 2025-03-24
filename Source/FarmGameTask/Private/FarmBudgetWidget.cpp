// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UFarmBudgetWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateFarmBudgetDisplay();
}

void UFarmBudgetWidget::UpdateFarmBudgetDisplay()
{
	// Safely get the current GameState
	AFarmGameTaskGameState* FarmGameState = Cast<AFarmGameTaskGameState>(UGameplayStatics::GetGameState(this));
	if (!FarmGameState || !BudgetTextBlock)
	{
		return;
	}

	// Get the current budget
	const int32 CurrentBudget = FarmGameState->FarmBudget;
	// Convert it to text and set on the BudgetTextBlock
	FText BudgetText = FText::AsNumber(CurrentBudget);
	BudgetTextBlock->SetText(BudgetText);

	// Get the current budget
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && CropsHarvested && MoneySpent && CropsProcessed)
	{
		AFarmGameTaskPlayerState* PS = PC->GetPlayerState<AFarmGameTaskPlayerState>();
		if (PS)
		{
			int32 Temp = PS->GetCropsHarvested();
			// Convert it to text and set on the BudgetTextBlock
			FText TempText = FText::AsNumber(Temp);
			CropsHarvested->SetText(TempText);
			
			Temp = PS->GetMoneySpent();
			TempText = FText::AsNumber(Temp);
			MoneySpent->SetText(TempText);

			Temp = PS->GetCropsProcessed();
			TempText = FText::AsNumber(Temp);
			CropsProcessed->SetText(TempText);
		}
	}
}

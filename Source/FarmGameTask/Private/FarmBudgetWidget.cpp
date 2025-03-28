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

	const int32 CurrentBudget = FarmGameState->FarmBudget;
	FText BudgetText = FText::AsNumber(CurrentBudget);
	BudgetTextBlock->SetText(BudgetText);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && MoneySpent && CropsHarvested)
	{
		AFarmGameTaskPlayerState* PS = PC->GetPlayerState<AFarmGameTaskPlayerState>();
		if (PS)
		{
			int32 Temp = PS->GetMoneySpent();
			FText TempText = FText::AsNumber(Temp);
			MoneySpent->SetText(TempText);

			Temp = PS->GetCropsHarvested();
			TempText = FText::AsNumber(Temp);
			CropsHarvested->SetText(TempText);
		}
	}
}

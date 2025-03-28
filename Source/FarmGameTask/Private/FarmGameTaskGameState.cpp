// Fill out your copyright notice in the Description page of Project Settings.


// #include "FarmGameTaskGameState.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"

#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "Net/UnrealNetwork.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Engine/Engine.h"

AFarmGameTaskGameState::AFarmGameTaskGameState()
{
	bReplicates = true;
	FarmBudget = 1000;
}

void AFarmGameTaskGameState::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (HasAuthority())
	{
	}
}

void AFarmGameTaskGameState::ServerChangeBudget_Implementation(int32 Delta)
{
	FarmBudget += Delta;

	UE_LOG(LogTemp, Log, TEXT("Farm budget changed by %d. New budget: %d."), Delta, FarmBudget);
	AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->FarmBudgetWidgetInstance->UpdateFarmBudgetDisplay();
	if (PC->SalesCounterWidgetInstance) PC->UpdateSalesWidget();

}

bool AFarmGameTaskGameState::ServerChangeBudget_Validate(int32 Delta)
{
	if (Delta > 1000 || Delta < -1000) return false;
	return true;
}

void AFarmGameTaskGameState::OnRep_FarmBudget()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_FarmBudget: Client sees new budget = %d."), FarmBudget);
	AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->FarmBudgetWidgetInstance->UpdateFarmBudgetDisplay();
	if (PC->SalesCounterWidgetInstance) PC->UpdateSalesWidget();
}

void AFarmGameTaskGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFarmGameTaskGameState, FarmBudget);
}
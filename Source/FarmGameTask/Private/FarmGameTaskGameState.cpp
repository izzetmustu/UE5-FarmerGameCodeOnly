// Fill out your copyright notice in the Description page of Project Settings.


// #include "FarmGameTaskGameState.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"

#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "Net/UnrealNetwork.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Engine/Engine.h"

AFarmGameTaskGameState::AFarmGameTaskGameState()
{
	// By default, GameState classes replicate. You can confirm here if needed.
	bReplicates = true;

	// Initialize the shared budget to some default (e.g. 1000).
	FarmBudget = 1000;
}

void AFarmGameTaskGameState::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (HasAuthority())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Initial FarmBudget is %d."), FarmBudget));
	}
}

void AFarmGameTaskGameState::ServerChangeBudget_Implementation(int32 Delta)
{
	// Only the server runs this logic. Adjust the shared budget by Delta.
	FarmBudget += Delta;

	// (Optional) You can clamp the budget if you wish (e.g., no negative budget):
	// FarmBudget = FMath::Max(FarmBudget, 0);

	// Log for debug
	UE_LOG(LogTemp, Log, TEXT("Farm budget changed by %d. New budget: %d."), Delta, FarmBudget);
	AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->FarmBudgetWidgetInstance->UpdateFarmBudgetDisplay();

}

bool AFarmGameTaskGameState::ServerChangeBudget_Validate(int32 Delta)
{
	if (Delta > 1000 || Delta < -1000) return false;
	return true;
}

void AFarmGameTaskGameState::OnRep_FarmBudget()
{
	// This runs on the CLIENT when FarmBudget is updated on the server.
	// Any UI updates or audio cues for budget changes can go here.
	UE_LOG(LogTemp, Log, TEXT("OnRep_FarmBudget: Client sees new budget = %d."), FarmBudget);
	// Get the local player controllers on this client and tell them to update the UI
	// for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	// {
	// 	AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(It->Get());
	// 	if (PC && PC->FarmBudgetWidgetInstance)
	// 	{
	// 		PC->FarmBudgetWidgetInstance->UpdateFarmBudgetDisplay();
	// 	}
	// }
	AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->FarmBudgetWidgetInstance->UpdateFarmBudgetDisplay();
}

void AFarmGameTaskGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate FarmBudget so all clients get the updated value
	DOREPLIFETIME(AFarmGameTaskGameState, FarmBudget);
}
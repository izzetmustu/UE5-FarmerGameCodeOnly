// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "Net/UnrealNetwork.h"

AFarmGameTaskPlayerState::AFarmGameTaskPlayerState()
{
    // Set default values
    CropsHarvested = 0;
    MoneySpent = 0;
    FarmScore = 0;
}

//------------------------------------------------
// Server RPC Implementations
//------------------------------------------------

void AFarmGameTaskPlayerState::ServerIncrementCropsHarvested_Implementation(int32 Amount)
{
    // Only the server runs this function body
    CropsHarvested += Amount;
    // Optionally clamp to a minimum/maximum if desired
    // e.g., CropsHarvested = FMath::Max(0, CropsHarvested);
}

bool AFarmGameTaskPlayerState::ServerIncrementCropsHarvested_Validate(int32 Amount)
{
    // Optional cheat-prevention or validation
    return (Amount >= 0); // Don’t allow negative increments
}

void AFarmGameTaskPlayerState::ServerIncrementMoneySpent_Implementation(int32 Amount)
{
    MoneySpent += Amount;
}

bool AFarmGameTaskPlayerState::ServerIncrementMoneySpent_Validate(int32 Amount)
{
    return (Amount >= 0);
}

void AFarmGameTaskPlayerState::ServerIncrementFarmScore_Implementation(int32 Amount)
{
    FarmScore += Amount;
}

bool AFarmGameTaskPlayerState::ServerIncrementFarmScore_Validate(int32 Amount)
{
    return (Amount != 0);
}

//------------------------------------------------
// OnRep_ (RepNotify) Functions
//------------------------------------------------

void AFarmGameTaskPlayerState::OnRep_CropsHarvested()
{
    // Runs on the client. Could update a HUD element or trigger an animation
    UE_LOG(LogTemp, Log, TEXT("OnRep_CropsHarvested: Now = %d for player %s"), CropsHarvested, *GetPlayerName());
}

void AFarmGameTaskPlayerState::OnRep_MoneySpent()
{
    UE_LOG(LogTemp, Log, TEXT("OnRep_MoneySpent: Now = %d for player %s"), MoneySpent, *GetPlayerName());
}

void AFarmGameTaskPlayerState::OnRep_FarmScore()
{
    UE_LOG(LogTemp, Log, TEXT("OnRep_FarmScore: Now = %d for player %s"), FarmScore, *GetPlayerName());
}

//------------------------------------------------
// Replication
//------------------------------------------------

void AFarmGameTaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFarmGameTaskPlayerState, CropsHarvested);
    DOREPLIFETIME(AFarmGameTaskPlayerState, MoneySpent);
    DOREPLIFETIME(AFarmGameTaskPlayerState, FarmScore);
}
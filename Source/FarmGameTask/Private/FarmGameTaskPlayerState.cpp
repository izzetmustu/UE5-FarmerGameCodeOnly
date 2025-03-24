#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Net/UnrealNetwork.h"

AFarmGameTaskPlayerState::AFarmGameTaskPlayerState()
{
    CornCount = 0;
    WheatCount = 0;
    ProcessedCornCount = 0;
    ProcessedWheatCount = 0;
    MoneySpent = 0;
    CropsHarvested = 0;
    CropsProcessed = 0;
    
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

void AFarmGameTaskPlayerState::ServerIncrementCropsProcessed_Implementation(int32 Amount)
{
    CropsProcessed += Amount;
}

bool AFarmGameTaskPlayerState::ServerIncrementCropsProcessed_Validate(int32 Amount)
{
    return (Amount != 0);
}

void AFarmGameTaskPlayerState::ServerAddToInventory_Implementation(ECropType ItemType, int32 Amount)
{
    switch (ItemType)
    {
    case ECropType::Corn:
        CornCount += Amount;
        break;

    case ECropType::Wheat:
        WheatCount += Amount;
        break;

        // If you have more items, add cases here...
    default:
        break;
    }

    if (AController* OwningController = Cast<AController>(GetOwner()))
    {
        // For a player-controlled pawn:
        if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(OwningController))
        {
            if (PC)
            {
                if (PC->bIsSalesWidgetVisible)
                {
                    PC->UpdateSalesWidget();          
                }
            }
        }
    }


}

bool AFarmGameTaskPlayerState::ServerAddToInventory_Validate(ECropType ItemType, int32 Amount)
{
    return (Amount > 0);
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

void AFarmGameTaskPlayerState::OnRep_CropsProcessed()
{
    UE_LOG(LogTemp, Log, TEXT("OnRep_CropsProcessed: Now = %d for player %s"), CropsProcessed, *GetPlayerName());
}

void AFarmGameTaskPlayerState::OnRep_Inventory()
{
    if (AController* OwningController = Cast<AController>(GetOwner()))
    {
        // For a player-controlled pawn:
        if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(OwningController))
        {
            if (PC)
            {
                if (PC->bIsSalesWidgetVisible)
                {
                    PC->UpdateSalesWidget();          
                }
            }
        }
    }
}

void AFarmGameTaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFarmGameTaskPlayerState, CornCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, WheatCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, ProcessedCornCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, ProcessedWheatCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, CropsHarvested);
    DOREPLIFETIME(AFarmGameTaskPlayerState, MoneySpent);
    DOREPLIFETIME(AFarmGameTaskPlayerState, CropsProcessed);
}

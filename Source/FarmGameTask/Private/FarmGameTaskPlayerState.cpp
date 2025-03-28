#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Net/UnrealNetwork.h"

AFarmGameTaskPlayerState::AFarmGameTaskPlayerState()
{
    CornCount = 0;
    WheatCount = 0;
    HarvestedCornCount = 0;
    HarvestedWheatCount = 0;
    MoneySpent = 0;
    CropsHarvested = 0;
}

void AFarmGameTaskPlayerState::ServerMoneySpent_Implementation(int32 Amount)
{
    MoneySpent += Amount;
}

bool AFarmGameTaskPlayerState::ServerMoneySpent_Validate(int32 Amount)
{
    return true;
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
    case ECropType::HarvestedCorn:
        HarvestedCornCount += Amount;
        CropsHarvested += Amount;
        break;
    case ECropType::HarvestedWheat:
        HarvestedWheatCount += Amount;
        CropsHarvested += Amount;
        break;
    default:
        break;
    }

    if (AController* OwningController = Cast<AController>(GetOwner()))
    {
        if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(OwningController))
        {
            if (PC)
            {
                if (PC->SalesCounterWidgetInstance)
                {
                    PC->UpdateSalesWidget();          
                }
            }
        }
    }
}

bool AFarmGameTaskPlayerState::ServerAddToInventory_Validate(ECropType ItemType, int32 Amount)
{
    return true;
}

void AFarmGameTaskPlayerState::OnRep_MoneySpent()
{
    if (AController* OwningController = Cast<AController>(GetOwner()))
    {
        if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(OwningController))
        {
            if (PC)
            {
                PC->UpdateFarmWidget();
            }
        }
    }
}

void AFarmGameTaskPlayerState::OnRep_Inventory()
{
    if (AController* OwningController = Cast<AController>(GetOwner()))
    {
        if (AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(OwningController))
        {
            if (PC)
            {
                PC->UpdateSalesWidget();          
                PC->UpdateFarmWidget();
            }
        }
    }
}

int32 AFarmGameTaskPlayerState::GetCropCount(ECropType CropType) const
{
    switch (CropType)
    {
    case ECropType::Corn:
        return CornCount;
    case ECropType::Wheat:
        return WheatCount;
    case ECropType::HarvestedCorn:
        return  HarvestedCornCount;
    case ECropType::HarvestedWheat:
        return HarvestedWheatCount;
    default:
        return CornCount;
    }
}

void AFarmGameTaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFarmGameTaskPlayerState, CornCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, WheatCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, HarvestedCornCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, HarvestedWheatCount);
    DOREPLIFETIME(AFarmGameTaskPlayerState, MoneySpent);
    DOREPLIFETIME(AFarmGameTaskPlayerState, CropsHarvested);
}

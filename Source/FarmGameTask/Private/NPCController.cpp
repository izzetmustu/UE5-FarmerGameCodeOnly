#include "NPCController.h"
#include "WayPoint.h"
#include "SalesCounter.h"
#include "CropTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "Navigation/PathFollowingComponent.h"
#include "FarmGameTaskGameState.h"

ANPCController::ANPCController()
{
    bGoingToBuy = false;
    bWandering = false;
    SalesCounterRef = nullptr;
}

void ANPCController::BeginPlay()
{
    Super::BeginPlay();
    
    TArray<AActor*> FoundWaypoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), FoundWaypoints);
    Waypoints = FoundWaypoints;
    if (Waypoints.Num() > 0)
    {
        for (AActor* Waypoint : Waypoints)
        {
            UE_LOG(LogTemp, Display, TEXT("Waypoint found: %s"), *Waypoint->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No waypoints found!"));
    }
}

void ANPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetNetMode() != NM_Client)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ANPCController::CheckSalesItems);
		GetWorldTimerManager().SetTimer(
			SalesCheckTimerHandle, 
			this, 
			&ANPCController::CheckSalesItems, 
			5.0f, 
			true
		);
	}
}

void ANPCController::CheckSalesItems()
{
    if (!SalesCounterRef)
    {
        TArray<AActor*> FoundCounters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASalesCounter::StaticClass(), FoundCounters);
        if (FoundCounters.Num() > 0)
        {
            SalesCounterRef = Cast<ASalesCounter>(FoundCounters[0]);
        }
    }

    if (!SalesCounterRef) return;

    const int32 CornOnCounter  = SalesCounterRef->GetCropCount(ECropType::HarvestedCorn);
    const int32 WheatOnCounter = SalesCounterRef->GetCropCount(ECropType::HarvestedWheat);

    const bool bItemsAvailable = (CornOnCounter + WheatOnCounter) > 0;
    UE_LOG(LogTemp, Display, TEXT("item available: %d"), bItemsAvailable);
    UE_LOG(LogTemp, Display, TEXT("bGoingToBuy: %d"), bGoingToBuy);

    if (bItemsAvailable)
    {
        if (bGoingToBuy)
        {
            UE_LOG(LogTemp, Display, TEXT("Already on the way to buy items"));
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Going to buy items"));
            bGoingToBuy = true;
            bWandering = false;
            GoToSalesCounter();
        }
    }
    else
    {
        if (bWandering)
        {
            UE_LOG(LogTemp, Display, TEXT("Already on the way to random waypoint"));
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Going to random waypoint"));
            bWandering = true;
            bGoingToBuy = false;
            GoToRandomWaypoint();
        }
    }

    
    // if (bItemsAvailable && !bGoingToBuy)
    // {
    //     UE_LOG(LogTemp, Display, TEXT("Going to buy items"));
    //     bGoingToBuy = true;
    //     GoToSalesCounter();
    // }
    // else if (!bItemsAvailable && !bWandering)
    // {
    //     UE_LOG(LogTemp, Display, TEXT("Going to random waypoint"));
    //     bGoingToBuy = false;
    //     bWandering  = true;
    //     GoToRandomWaypoint();
    // }
    // else{
    //     UE_LOG(LogTemp, Display, TEXT("On the way to random waypoint"));       
    // }
    
}

void ANPCController::GoToRandomWaypoint()
{
    // bGoingToBuy = false;
    // bWandering = true;
    if (Waypoints.Num() > 0)
    {
        AActor* RandomPoint = Waypoints[FMath::RandRange(0, Waypoints.Num() - 1)];
        MoveToActor(RandomPoint, 25.f); 
    }
    else
    {
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys)
        {
            UE_LOG(LogTemp, Display, TEXT("NavSys found, moving to random location"));
            FNavLocation RandomLoc;
            if (NavSys->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000.f, RandomLoc))
            {
                MoveToLocation(RandomLoc.Location, 5.f);
            }
        }
    }
}

void ANPCController::GoToSalesCounter()
{
    if (SalesCounterRef)
    {
        UE_LOG(LogTemp, Display, TEXT("Sales Counter is valid, moving to it"));
        FAIRequestID RequestID = MoveToLocation(SalesCounterRef->GetActorLocation()+FVector(-150.f, 0.0f, 0.0f), 0.f);
        UE_LOG(LogTemp, Log, TEXT("MoveToActor returned request ID: %d"), RequestID.GetID());
    }
}

void ANPCController::BuyItems()
{
    if (!SalesCounterRef) return;


    // change budget
    AFarmGameTaskGameState* FarmGS = GetWorld()->GetGameState<AFarmGameTaskGameState>();
    if (FarmGS)
    {
        int32 CornToBuy = FMath::RandRange(0, SalesCounterRef->GetCropCount(ECropType::HarvestedCorn));
        int32 WheatToBuy = FMath::RandRange(0, SalesCounterRef->GetCropCount(ECropType::HarvestedWheat));
        int32 TotalCost = (CornToBuy * SalesCounterRef->GetCropPrice(ECropType::HarvestedCorn)) +
                          (WheatToBuy * SalesCounterRef->GetCropPrice(ECropType::HarvestedWheat));

        SalesCounterRef->ServerChangeStock(ECropType::HarvestedCorn, -CornToBuy);
        SalesCounterRef->ServerChangeStock(ECropType::HarvestedWheat, -WheatToBuy);
        FarmGS->ServerChangeBudget(TotalCost);
    }
    
    
    bGoingToBuy = false;
    bWandering = false;
    // GoToRandomWaypoint();
}

void ANPCController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    UE_LOG(LogTemp, Log, TEXT("OnMoveCompleted: Code=%d, IsSuccess=%d"), 
           (int32)Result.Code, Result.IsSuccess());
    
    if (Result.IsSuccess() && SalesCounterRef && bGoingToBuy)
    {
        if (FVector::Dist(GetPawn()->GetActorLocation(), SalesCounterRef->GetActorLocation()) < 200.f)
        {
            BuyItems();
        }
    }
    else
    {
        // for (AActor* Waypoint : Waypoints)
        // {
        //     if (FVector::Dist(GetPawn()->GetActorLocation(), Waypoint->GetActorLocation()) < 200.f)
        //     {
        //         bWandering = false;
        //         bGoingToBuy = false;
        //         break;
        //     }
        // }
        bWandering = false;
        bGoingToBuy = false;
    }
}
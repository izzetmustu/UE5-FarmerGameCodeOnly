// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmGameTask/Public/SalesCounter.h"

#include "FarmGameTaskPlayerController.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASalesCounter::ASalesCounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // Ensure replication

    // Create the Mesh Component
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    
    OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
    OverlapBox->SetupAttachment(RootComponent);
    OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    
	// Default counts
	WheatCount = 100;
	CornCount  = 100;

}

// Called when the game starts or when spawned
void ASalesCounter::BeginPlay()
{
	Super::BeginPlay();
    if (CounterMaterial)
    {
        MeshComp->SetMaterial(0, CounterMaterial);
    }

    OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ASalesCounter::OnOverlapBoxBegin);
    OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ASalesCounter::OnOverlapBoxEnd);
}

void ASalesCounter::OnOverlapBoxBegin(
    UPrimitiveComponent* OverlappedComp, 
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex,
    bool bFromSweep, 
    const FHitResult & SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Log, TEXT("SalesCounter Overlap Begin: %s"), *OtherActor->GetName());

        // If the overlapping actor is a PlayerCharacter or Pawn with a PlayerController:
        APawn* OverlappingPawn = Cast<APawn>(OtherActor);
        if (OverlappingPawn && OverlappingPawn->IsLocallyControlled())
        {
            APlayerController* PC = Cast<APlayerController>(OverlappingPawn->GetController());
            if (PC)
            {
                // Our custom farm PC
                if (AFarmGameTaskPlayerController* FarmPC = Cast<AFarmGameTaskPlayerController>(PC))
                {
                    FarmPC->ShowSalesCounter(this);
                }
            }
        }
    }
}

void ASalesCounter::OnOverlapBoxEnd(
    UPrimitiveComponent* OverlappedComp, 
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Log, TEXT("SalesCounter Overlap End: %s"), *OtherActor->GetName());

        APawn* OverlappingPawn = Cast<APawn>(OtherActor);
        if (OverlappingPawn && OverlappingPawn->IsLocallyControlled())
        {
            APlayerController* PC = Cast<APlayerController>(OverlappingPawn->GetController());
            if (PC)
            {
                if (AFarmGameTaskPlayerController* FarmPC = Cast<AFarmGameTaskPlayerController>(PC))
                {
                    FarmPC->HideSalesCounter();
                }
            }
        }
    }
}

void ASalesCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AFarmGameTaskGameState* ASalesCounter::GetFarmGameState() const
{
    return GetWorld() ? GetWorld()->GetGameState<AFarmGameTaskGameState>() : nullptr;
}

void ASalesCounter::ServerBuyCrop_Implementation(ECropType CropType, int32 Amount)
{
    if (WheatCount < 1 || CornCount < 1)
    {
        return;
    }
    
    int32 AvailableCount = 0;
    int32 PricePerUnit   = 0;
    switch (CropType)
    {
    case ECropType::Wheat:
        AvailableCount = WheatCount;
        PricePerUnit   = 10; // e.g., 10 currency per Wheat
        break;

    case ECropType::Corn:
        AvailableCount = CornCount;
        PricePerUnit   = 15; // e.g., 15 currency per Corn
        break;

    default:
        break;
    }
    
    int32 QuantityToBuy = FMath::Min(Amount, AvailableCount);
    if (CropType == ECropType::Wheat)
    {
        WheatCount -= QuantityToBuy;
    }
    else
    {
        CornCount -= QuantityToBuy;
    }

    int32 TotalSaleValue = QuantityToBuy * PricePerUnit;
    if (TotalSaleValue < GetFarmGameState()->GetFarmBudget())
    {
        if (AFarmGameTaskGameState* FarmGS = GetFarmGameState()){
            FarmGS->ServerChangeBudget(-TotalSaleValue);
        }
        UpdateDisplayWidget();
    }
}

bool ASalesCounter::ServerBuyCrop_Validate(ECropType CropType, int32 Amount)
{
    return (Amount > 0 && Amount < 999999);
}

void ASalesCounter::ServerPlaceCrop_Implementation(ECropType CropType, int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    switch (CropType)
    {
        case ECropType::Wheat:
            ProcessedWheatCount += Amount;
            UE_LOG(LogTemp, Log, TEXT("Placed %d Wheat on the counter. Total: %d"), Amount, WheatCount);
            break;

        case ECropType::Corn:
            ProcessedCornCount += Amount;
            UE_LOG(LogTemp, Log, TEXT("Placed %d Corn on the counter. Total: %d"), Amount, CornCount);
            break;

        default:
            break;
    }
    UpdateDisplayWidget();
}

bool ASalesCounter::ServerPlaceCrop_Validate(ECropType CropType, int32 Amount)
{
    // Optional cheat-prevention (e.g., disallow huge amounts)
    return (Amount > 0 && Amount < 999999);
}

void ASalesCounter::UpdateDisplayWidget()
{
    AFarmGameTaskPlayerController* PC = Cast<AFarmGameTaskPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PC)
    {
        if (PC->bIsSalesWidgetVisible)
        {
            PC->UpdateSalesWidget();          
        }
    }
}

void ASalesCounter::OnRep_WheatCount()
{
    UpdateDisplayWidget();
    UE_LOG(LogTemp, Log, TEXT("OnRep_WheatCount: New value = %d"), WheatCount);
}

void ASalesCounter::OnRep_CornCount()
{
    UpdateDisplayWidget();
    UE_LOG(LogTemp, Log, TEXT("OnRep_CornCount: New value = %d"), CornCount);
}

void ASalesCounter::OnRep_ProcessedWheat()
{
    UpdateDisplayWidget();
}

void ASalesCounter::OnRep_ProcessedCorn()
{
    UpdateDisplayWidget();
}

void ASalesCounter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASalesCounter, WheatCount);
    DOREPLIFETIME(ASalesCounter, CornCount);
}

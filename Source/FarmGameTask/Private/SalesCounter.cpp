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
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshComp && CubeMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(CubeMesh.Object);
        MeshComp->SetRelativeScale3D(FVector(1.0f, 6.0f, 1.0f));
    }
    
    OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
    OverlapBox->SetupAttachment(RootComponent);
    OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    OverlapBox->SetWorldScale3D(FVector(3.25f,1.75f,2.0f));
    OverlapBox->SetRelativeLocation(FVector(94.f, 1.8f, 10.f));

    // Create a default material for the sales counter
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/ThirdPerson/Materials/M_SalesCounter"));
    if (MaterialFinder.Succeeded())
    {
        CounterMaterial = MaterialFinder.Object;
        MeshComp->SetMaterial(0, CounterMaterial);
    }
    
	WheatCount = 100;
	CornCount  = 100;
    CropPrices.Add(ECropType::Wheat ,10);
    CropPrices.Add(ECropType::Corn ,15);
    CropPrices.Add(ECropType::ProcessedWheat ,30);
    CropPrices.Add(ECropType::ProcessedCorn ,45);

    SetNetUpdateFrequency(66.f);
    SetMinNetUpdateFrequency(33.f);
}

// Called when the game starts or when spawned
void ASalesCounter::BeginPlay()
{
	Super::BeginPlay();
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

void ASalesCounter::ServerChangeStock_Implementation(ECropType CropType, int32 Amount)
{
    switch (CropType)
    {
    case ECropType::Wheat:
        WheatCount += Amount;
        break;
    case ECropType::Corn:
        CornCount += Amount;
        break;
    case ECropType::ProcessedWheat:
        ProcessedWheatCount += Amount;
        break;
    case ECropType::ProcessedCorn:
        ProcessedCornCount += Amount;
    default:
        break;
    }
}

bool ASalesCounter::ServerChangeStock_Validate(ECropType CropType, int32 Amount)
{
    return true;
}

int32 ASalesCounter::GetCropCount(ECropType CropType) const
{
    switch (CropType)
    {
    case ECropType::Corn:
        return CornCount;
    case ECropType::Wheat:
        return WheatCount;
    case ECropType::ProcessedCorn:
        return  ProcessedCornCount;
    case ECropType::ProcessedWheat:
        return ProcessedWheatCount;
    default:
        return CornCount;
    }
}

void ASalesCounter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASalesCounter, WheatCount);
    DOREPLIFETIME(ASalesCounter, CornCount);
}

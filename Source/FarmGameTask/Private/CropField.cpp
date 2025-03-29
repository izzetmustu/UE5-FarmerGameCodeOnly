#include "FarmGameTask/Public/CropField.h"
#include "FarmGameTask/Public/CropSlot.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "FarmGameTask/Public/SalesCounterWidget.h"
#include "FarmGameTask/Public/InteractSeedWidget.h"
#include "FarmGameTask/Public/SalesCounter.h"
#include "FarmGameTask/Public/CropTypes.h"
#include "Net/UnrealNetwork.h"

ACropField::ACropField()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Slots.Reserve(25);
	SlotClass = ACropSlot::StaticClass();
}

void ACropField::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!RootComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("RootComponent missing in PostInitializeComponents! Creating fallback root."));
		USceneComponent* FallbackRoot = NewObject<USceneComponent>(this, TEXT("FallbackRoot"));
		FallbackRoot->RegisterComponent();
		SetRootComponent(FallbackRoot);
	}

	SetReplicates(true);       
}

void ACropField::BeginPlay()
{
	Super::BeginPlay();
	if (!SlotClass) return;
	if (HasAuthority()) 
	{
		const int32 GridSizeX = 5;
		const int32 GridSizeY = 5;
		const float SlotSpacing = 110.0f;

		for (int32 X = 0; X < GridSizeX; ++X)
		{
			for (int32 Y = 0; Y < GridSizeY; ++Y)
			{
				FVector SpawnLoc = GetActorLocation() + FVector(X * SlotSpacing, Y * SlotSpacing, 0.0f);

				FActorSpawnParameters Params;
				Params.Owner = this;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ACropSlot* SpawnedSlot = GetWorld()->SpawnActor<ACropSlot>(SlotClass, SpawnLoc, FRotator::ZeroRotator, Params);
				if (SpawnedSlot)
				{
					SpawnedSlot->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
					Slots.Add(SpawnedSlot);
				}
			}
		}
	}
	
}

void ACropField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACropField::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACropField, Slots);
}


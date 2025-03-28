#include "CropSlot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"

ACropSlot::ACropSlot()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SlotInfo = FSlotInfo();
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Mesh && CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
		Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.1f));
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // Replace with your actual custom channel

	// Set material to the crop slot material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/ThirdPerson/Materials/M_CropSlot"));
	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
		Mesh->SetMaterial(0, Material);
	}
	
}

void ACropSlot::BeginPlay()
{
	Super::BeginPlay();
	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (DynamicMaterial)
		{
			Mesh->SetMaterial(0, DynamicMaterial); // Override the static material
		}
	}
	ClearTimer();
}

void ACropSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACropSlot::SetSlotInfo_Implementation(FSlotInfo InSlotInfo)
{
	if (SlotInfo.State == ESlotState::Growing || SlotInfo.State == ESlotState::ReadyToHarvest) return;
	ClearTimer();
	SlotInfo = InSlotInfo;
	StartTimer();
}

bool ACropSlot::SetSlotInfo_Validate(FSlotInfo InSlotInfo)
{
	return true;
}


void ACropSlot::ClearTimer()
{
	if (HasAuthority()) GetWorld()->GetTimerManager().ClearTimer(GrowthTimerHandle);
}

void ACropSlot::StartTimer()
{
	if (HasAuthority()) GetWorld()->GetTimerManager().SetTimer(GrowthTimerHandle, this, &ACropSlot::OnGrowthTick, 1.0f, true);
}

void ACropSlot::OnGrowthTick()
{
	if (!HasAuthority()) return;

	FSlotInfo NewSlotInfo = SlotInfo;
	if (NewSlotInfo.State == ESlotState::ReadyToHarvest)
	{
		NewSlotInfo.TimeRemaining -= 1.0f;
		if (NewSlotInfo.TimeRemaining <= -10)
		{
			NewSlotInfo.State = ESlotState::Damaged;
		}
	}
	else if (NewSlotInfo.State == ESlotState::Growing)
	{
		NewSlotInfo.TimeRemaining -= 1.0f;
		if (NewSlotInfo.TimeRemaining <= 0)
		{
			NewSlotInfo.State = ESlotState::ReadyToHarvest;
		}
	}
	else if (NewSlotInfo.State == ESlotState::Damaged)
	{
		NewSlotInfo.TimeRemaining -= 1.0f;
	}
	else
	{
		NewSlotInfo.State = ESlotState::Empty;
		NewSlotInfo.TimeRemaining = 0;
		NewSlotInfo.SeedType = ESlotSeedType::None;
		ClearTimer();
	}

	SlotInfo = NewSlotInfo;
	UpdateVisuals();
}

void ACropSlot::OnRep_SlotInfo()
{
	UpdateVisuals();
}

void ACropSlot::UpdateVisuals()
{
	if (SlotInfo.State == ESlotState::Empty)
	{
		FVector NewScale = FVector(1.0f, 1.0f, 0.1f);
		Mesh->SetRelativeScale3D(NewScale);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor::White);
		}
	}
	else if (SlotInfo.State == ESlotState::Growing)
	{
		float Progress = SlotInfo.TimeRemaining;
		float Diff = (0.5f - Mesh->GetRelativeScale3D().Z)/SlotInfo.TimeRemaining;
		FVector NewScale = FVector(1.0f, 1.0f, Mesh->GetRelativeScale3D().Z + Diff);
		Mesh->SetRelativeScale3D(NewScale);

		float Ratio = NewScale.Z / 0.5f; // Assuming 0.5 is the max scale
		FLinearColor NewColor = FLinearColor::LerpUsingHSV(FLinearColor::White, FLinearColor::Green, Ratio);// Update material color
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", NewColor);
		}

	}
	else if (SlotInfo.State == ESlotState::ReadyToHarvest)
	{
		Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.5f));
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor::Green);
		}
	}
	else
	{
		Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.5f));
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor::Red);
		}
	}
}

void ACropSlot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACropSlot, SlotInfo);
}
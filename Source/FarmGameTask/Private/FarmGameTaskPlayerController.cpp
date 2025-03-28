#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "FarmGameTask/Public/SalesCounter.h"
#include "FarmGameTask/Public/SalesCounterWidget.h"
#include "FarmGameTask/Public/InteractSeedWidget.h"
#include "FarmGameTask/Public/CropTypes.h"
#include "FarmGameTask/Public/CropSlot.h"
// #include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FarmGameTaskGameState.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AFarmGameTaskPlayerController::AFarmGameTaskPlayerController()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> FarmBudgetWidgetBPClass(TEXT("/Game/ThirdPerson/Blueprints/WBP_FarmBudgetWidget"));
    if (FarmBudgetWidgetBPClass.Class != NULL)
    {
        FarmBudgetWidgetClass = FarmBudgetWidgetBPClass.Class;
        FarmBudgetWidgetInstance = nullptr;
    }
    static ConstructorHelpers::FClassFinder<UUserWidget> USalesCounterWidgetBPClass(TEXT("/Game/ThirdPerson/Blueprints/WBP_SalesCounterWidget"));
    if (USalesCounterWidgetBPClass.Class != NULL)
    {
        SalesCounterWidgetClass = USalesCounterWidgetBPClass.Class;
        SalesCounterWidgetInstance = nullptr;
    }
    static ConstructorHelpers::FClassFinder<UUserWidget> UInteractSeedWidgetClass(TEXT("/Game/ThirdPerson/Blueprints/WBP_InteractSeedWidget"));
    if (UInteractSeedWidgetClass.Class != NULL)
    {
        InteractSeedWidgetClass = UInteractSeedWidgetClass.Class;
        InteractSeedWidgetInstance = nullptr;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFinder(TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact"));
    if (InputActionFinder.Succeeded())
    {
        IA_Interact = InputActionFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextFinder(TEXT("/Game/ThirdPerson/Input/IMC_Default"));
    
    if (MappingContextFinder.Succeeded())
    {
        IMC_MyMappings = MappingContextFinder.Object;
    }
    bIsSalesWidgetVisible = false;
}

void AFarmGameTaskPlayerController::BeginPlay()
{
    Super::BeginPlay();

    
    if (IsLocalController() && FarmBudgetWidgetClass) // Only create the widget on local players
    {
        FarmBudgetWidgetInstance = CreateWidget<UFarmBudgetWidget>(this, FarmBudgetWidgetClass);
        if (FarmBudgetWidgetInstance)
        {
            FarmBudgetWidgetInstance->AddToViewport();
        }
    }
    if (IsLocalController() && IMC_MyMappings)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(IMC_MyMappings, 0);
        }
    }
}

void AFarmGameTaskPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red,FString::Printf(TEXT("InputComponent is null")));
        return;
    }

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInput->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &AFarmGameTaskPlayerController::Interact);
    }

}

void AFarmGameTaskPlayerController::Interact()
{
    if (!IsLocalController()) return;
    
    FHitResult HitResult;
    FVector Start = GetPawn()->GetActorLocation();
    FVector End = Start - FVector(0, 0, 1000.0f);
    UE_LOG(LogTemp, Warning, TEXT("interact: %s") , *Start.ToString());
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignore self

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,        
        Start,             
        End,              
        ECC_GameTraceChannel1,  
        CollisionParams 
    );

    if (bHit)
    {
        ACropSlot* CropSlot = Cast<ACropSlot>(HitResult.GetActor());
        if (CropSlot)
        {
            if (CropSlot->GetSlotInfo().TimeRemaining > 0.0f) return;
            if (InteractSeedWidgetInstance && InteractSeedWidgetInstance->IsInViewport())
            {
                InteractSeedWidgetInstance->RemoveFromParent();
                InteractSeedWidgetInstance = nullptr;
            }
            InteractSeedWidgetInstance = CreateWidget<UInteractSeedWidget>(this, InteractSeedWidgetClass);
            if (InteractSeedWidgetInstance)
            {
                InteractSeedWidgetInstance->AddToViewport();
                InteractSeedWidgetInstance->SetCropSlot(CropSlot);
            }
        }

    }
}

void AFarmGameTaskPlayerController::ServerInteractWithSlot_Implementation(ACropSlot* TargetSlot, FSlotInfo Info)
{
    if (TargetSlot)
    {
        TargetSlot->SetSlotInfo(Info);
    }
}

bool AFarmGameTaskPlayerController::ServerInteractWithSlot_Validate(ACropSlot* TargetSlot, FSlotInfo Info)
{
    return true;
}

void AFarmGameTaskPlayerController::ServerAttemptSowSlot_Implementation(ACropSlot* TargetSlot, FSlotInfo Info)
{
    AFarmGameTaskPlayerState* PS = GetPlayerState<AFarmGameTaskPlayerState>();
    if (!TargetSlot || !PS) return;
    ECropType SeedType = Info.SeedType == ESlotSeedType::Wheat ? ECropType::Wheat : ECropType::Corn;
    PS->ServerAddToInventory(SeedType, -1);
    ServerInteractWithSlot(TargetSlot, Info);
}

bool AFarmGameTaskPlayerController::ServerAttemptSowSlot_Validate(ACropSlot* TargetSlot, FSlotInfo Info)
{
    return true;
}

void AFarmGameTaskPlayerController::ServerAttemptHarvestSlot_Implementation(ACropSlot* TargetSlot)
{
    AFarmGameTaskPlayerState* PS = GetPlayerState<AFarmGameTaskPlayerState>();
    if (!TargetSlot || !PS) return;
    if (TargetSlot->GetSlotInfo().State != ESlotState::Damaged)
    {
        ECropType SeedType = TargetSlot->GetSlotInfo().SeedType == ESlotSeedType::Wheat ? ECropType::HarvestedWheat : ECropType::HarvestedCorn;
        PS->ServerAddToInventory(SeedType, 1);
    }
    ServerInteractWithSlot(TargetSlot, FSlotInfo());
}

bool AFarmGameTaskPlayerController::ServerAttemptHarvestSlot_Validate(ACropSlot* TargetSlot)
{
    return true;
}

void AFarmGameTaskPlayerController::ServerAttemptBuyCrop_Implementation(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    if (!TargetCounter || Amount <= 0)
    {
        return;
    }
    
    AFarmGameTaskGameState* FarmGS = GetWorld()->GetGameState<AFarmGameTaskGameState>();
    AFarmGameTaskPlayerState* PS = GetPlayerState<AFarmGameTaskPlayerState>();
    if (!FarmGS || !PS)
    {
        return; 
    }

    int32 CropAvailable = TargetCounter->GetCropCount(CropType);
    int32 TotalCost = Amount*TargetCounter->GetCropPrice(CropType);
    
    if (CropAvailable < Amount || FarmGS->GetFarmBudget() < TotalCost)
    {
        return;
    }

    TargetCounter->ServerChangeStock(CropType, -Amount);
    PS->ServerAddToInventory(CropType, Amount);
    PS->ServerMoneySpent(TargetCounter->GetCropPrice(CropType));
    FarmGS->ServerChangeBudget(-TotalCost);
}
bool AFarmGameTaskPlayerController::ServerAttemptBuyCrop_Validate(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    return true;
}

void AFarmGameTaskPlayerController::ServerAttemptPlaceCrop_Implementation(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    if (!TargetCounter || Amount <= 0)
    {
        return;
    }
    
    AFarmGameTaskGameState* FarmGS = GetWorld()->GetGameState<AFarmGameTaskGameState>();
    AFarmGameTaskPlayerState* PS = GetPlayerState<AFarmGameTaskPlayerState>();
    if (!FarmGS || !PS)
    {
        return; 
    }

    int32 CropAvailable = PS->GetCropCount(CropType);
    int32 TotalCost = CropAvailable*TargetCounter->GetCropPrice(CropType);
    if (CropAvailable < Amount)
    {
        return;
    }

    TargetCounter->ServerChangeStock(CropType, Amount);
    PS->ServerAddToInventory(CropType, -Amount);
    FarmGS->ServerChangeBudget(TotalCost);
}
bool AFarmGameTaskPlayerController::ServerAttemptPlaceCrop_Validate(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    return true;
}

void AFarmGameTaskPlayerController::UpdateSalesWidget()
{
    if (SalesCounterWidgetInstance) SalesCounterWidgetInstance->UpdateCounterDisplay();
}

void AFarmGameTaskPlayerController::UpdateFarmWidget()
{
    if (FarmBudgetWidgetInstance) FarmBudgetWidgetInstance->UpdateFarmBudgetDisplay();
}

void AFarmGameTaskPlayerController::ShowSalesCounter(ASalesCounter* InSalesCounter)
{
    if (bIsSalesWidgetVisible)
        return;

    bIsSalesWidgetVisible = true;

    if (FarmBudgetWidgetInstance)
    {
        FarmBudgetWidgetInstance->RemoveFromParent();
    }

    if (SalesCounterWidgetClass)
    {
        SalesCounterWidgetInstance = CreateWidget<USalesCounterWidget>(this, SalesCounterWidgetClass);
        if (SalesCounterWidgetInstance)
        {
            SalesCounterWidgetInstance->SetSalesCounter(InSalesCounter);
            SalesCounterWidgetInstance->AddToViewport();
        }
    }

    bShowMouseCursor = true;
    SetIgnoreLookInput(true);
}

void AFarmGameTaskPlayerController::HideSalesCounter()
{
    if (!bIsSalesWidgetVisible)
        return;

    bIsSalesWidgetVisible = false;

    if (SalesCounterWidgetInstance)
    {
        if (SalesCounterWidgetInstance->IsInViewport())
        {
            SalesCounterWidgetInstance->RemoveFromParent();
        }
        SalesCounterWidgetInstance = nullptr;
    }
    bShowMouseCursor = false;
    SetIgnoreLookInput(false);

    if (FarmBudgetWidgetInstance && !FarmBudgetWidgetInstance->IsInViewport())
    {
        FarmBudgetWidgetInstance->AddToViewport();
    }
}

#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerState.h"
#include "FarmGameTask/Public/SalesCounter.h"
#include "FarmGameTask/Public/SalesCounterWidget.h"
#include "FarmGameTask/Public/CropTypes.h"
#include "Kismet/GameplayStatics.h"
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
    static ConstructorHelpers::FClassFinder<UUserWidget> USalesCounterWidget(TEXT("/Game/ThirdPerson/Blueprints/WBP_SalesCounterWidget"));
    if (USalesCounterWidget.Class != NULL)
    {
        SalesCounterWidgetClass = USalesCounterWidget.Class;
        SalesCounterWidgetInstance = nullptr;
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
}

void AFarmGameTaskPlayerController::BuySeeds(int32 SeedCost)
{
}

void AFarmGameTaskPlayerController::BeginPlay()
{
    Super::BeginPlay();

    
    if (IsLocalController() && FarmBudgetWidgetClass) // Only create the widget on local players
    {
        // Create and add the widget to the viewport
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

    // Safety check
    if (!InputComponent)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red,FString::Printf(TEXT("InputComponent is null")));
        return;
    }

    // Bind an Interact action to the 'E' key (Engine default name is "Interact" if you created an Input Mapping)
    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // Bind the Interact action
        EnhancedInput->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &AFarmGameTaskPlayerController::Interact);
    }

    // You could also bind movement or other actions here, e.g. "MoveForward", "MoveRight" if not handled by a Pawn/Character
}

void AFarmGameTaskPlayerController::Interact()
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Log, TEXT("SERVER CALLS ServerInteract_Implementation FROM INTERACT"));
        ServerInteract_Implementation();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("CLIENT CALLS ServerInteract FROM INTERACT"));
        ServerInteract();
    }
}

void AFarmGameTaskPlayerController::ServerInteract_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("ServerInteract_Implementation: Doing server-side interaction logic"));
    // BuySeeds(10);
}

bool AFarmGameTaskPlayerController::ServerInteract_Validate()
{
    return true;
}

void AFarmGameTaskPlayerController::ServerAttemptBuyCrop_Implementation(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    TargetCounter->ServerBuyCrop(CropType, Amount);
    AFarmGameTaskPlayerState* PS = GetPlayerState<AFarmGameTaskPlayerState>();
    if (PS)
    {
        PS->ServerAddToInventory(CropType, Amount);
    }
}
bool AFarmGameTaskPlayerController::ServerAttemptBuyCrop_Validate(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    return (TargetCounter && Amount > 0 && Amount < 999999);
}

void AFarmGameTaskPlayerController::ServerAttemptPlaceCrop_Implementation(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    if (TargetCounter)
    {
        TargetCounter->ServerPlaceCrop(CropType, Amount);
    }
}
bool AFarmGameTaskPlayerController::ServerAttemptPlaceCrop_Validate(ASalesCounter* TargetCounter, ECropType CropType, int32 Amount)
{
    return true;
}

void AFarmGameTaskPlayerController::UpdateSalesWidget()
{
    SalesCounterWidgetInstance->UpdateCounterDisplay();
}

void AFarmGameTaskPlayerController::ShowSalesCounter(ASalesCounter* InSalesCounter)
{
    // If we already have the sales widget visible, do nothing
    if (bIsSalesWidgetVisible)
        return;

    bIsSalesWidgetVisible = true;

    // Remove the farm budget widget from viewport (if it's displayed)
    if (FarmBudgetWidgetInstance)
    {
        FarmBudgetWidgetInstance->RemoveFromParent();
    }

    // Create the sales counter widget
    if (SalesCounterWidgetClass)
    {
        SalesCounterWidgetInstance = CreateWidget<USalesCounterWidget>(this, SalesCounterWidgetClass);
        if (SalesCounterWidgetInstance)
        {
            // Link the sales counter to the widget
            SalesCounterWidgetInstance->SetSalesCounter(InSalesCounter);
            SalesCounterWidgetInstance->AddToViewport();
        }
    }

    // (2) Show mouse cursor
    bShowMouseCursor = true;

    // // (3) Set an input mode focusing on UI
    // FInputModeUIOnly InputMode;
    // if (SalesCounterWidgetInstance)
    // {
    //     // Set the widget to focus so that keyboard/gamepad input goes to the UI
    //     InputMode.SetWidgetToFocus(SalesCounterWidgetInstance->TakeWidget());
    // }
    // SetInputMode(InputMode);

    // (4) Disable camera rotation/look
    SetIgnoreLookInput(true);  // Now moving mouse won't rotate camera
    // SetIgnoreMoveInput(false);  // Optionally keep movement if you want

    // If you want to disable *all* movement:
    // SetIgnoreMoveInput(true);
}

void AFarmGameTaskPlayerController::HideSalesCounter()
{
    // If the sales widget isn't visible, do nothing
    if (!bIsSalesWidgetVisible)
        return;

    bIsSalesWidgetVisible = false;

    // Remove the SalesCounterWidget from the viewport
    if (SalesCounterWidgetInstance)
    {
        SalesCounterWidgetInstance->RemoveFromParent();
        SalesCounterWidgetInstance = nullptr;  // Destroy or re-create next time
    }
    // (2) Hide the mouse cursor
    bShowMouseCursor = false;

    // // (3) Set input mode back to gameplay
    // FInputModeGameOnly GameInput;
    // SetInputMode(GameInput);

    // (4) Re-enable look input
    SetIgnoreLookInput(false);
    // SetIgnoreMoveInput(false); 

    // Re-add the FarmBudgetWidget
    if (FarmBudgetWidgetInstance && !FarmBudgetWidgetInstance->IsInViewport())
    {
        FarmBudgetWidgetInstance->AddToViewport();
    }
}

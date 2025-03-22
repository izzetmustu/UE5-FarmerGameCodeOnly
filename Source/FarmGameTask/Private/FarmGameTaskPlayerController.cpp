#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FarmGameTask/Public/FarmBudgetWidget.h"
#include "FarmGameTask/Public/FarmGameTaskGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AFarmGameTaskPlayerController::AFarmGameTaskPlayerController()
{
    // Optionally set default values or widget references here
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<UUserWidget> FarmBudgetWidgetBPClass(TEXT("/Game/ThirdPerson/Blueprints/WBP_FarmBudgetWidget"));
    if (FarmBudgetWidgetBPClass.Class != NULL)
    {
        FarmBudgetWidgetClass = FarmBudgetWidgetBPClass.Class;
        FarmBudgetWidgetInstance = nullptr;
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
    // This is the client function called when pressing 'E'
    // Typically you'd do a client-side line trace to find an interactable object, 
    // then pass it to the server to do the real interaction.
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
    
    // // Example of a direct server call:
    // if (!HasAuthority() && IsLocalController())
    // {
    //     ServerInteract();
    //     UWorld* World = GetWorld();
    //     if (World)
    //     {
    //         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("interact ")));
    //     }
    // }
    // else
    // {
    //     // If for some reason this is already the server, do the logic directly
    //     // But normally, the PlayerController on a Listen Server is authoritative too
    //     UE_LOG(LogTemp, Log, TEXT("Interact called on server already."));
    //     UWorld* World = GetWorld();
    //     if (World)
    //     {
    //         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("interact has authority ")));
    //     }
    // }
}

void AFarmGameTaskPlayerController::ServerInteract_Implementation()
{
    // Perform the actual interaction on the server (authoritative).
    // e.g. line trace from the player's Pawn, check if it hits an IInteractable,
    // then call Interact(...) on that object, subtract budget, etc.
    UE_LOG(LogTemp, Log, TEXT("ServerInteract_Implementation: Doing server-side interaction logic"));
    BuySeeds(10);
}

bool AFarmGameTaskPlayerController::ServerInteract_Validate()
{
    // Optionally do checks to prevent cheating (distance checks, etc.)
    return true;
}

//--------------------------------------------------
// Buying Seeds Example
//--------------------------------------------------
void AFarmGameTaskPlayerController::BuySeeds(int32 SeedCost)
{
    // This is a BlueprintCallable method that you might call from a UI button or other code
    if (!HasAuthority())
    {
        // If not authoritative, call the server
        ServerBuySeeds(SeedCost);
    }
    else
    {
        // If this is a Listen Server's local PC, we can just do the logic
        ServerBuySeeds_Implementation(SeedCost);
    }
}

void AFarmGameTaskPlayerController::ServerBuySeeds_Implementation(int32 Cost)
{
    // On the server: get the FarmGameState, reduce the budget
    AFarmGameTaskGameState* FarmGS = GetWorld()->GetGameState<AFarmGameTaskGameState>();
    if (FarmGS)
    {
        UE_LOG(LogTemp, Log, TEXT("ServerBuySeeds_Implementation: Buying seeds with cost=%d"), Cost);
        FarmGS->ServerChangeBudget(-Cost);  // Or clamp so it doesn't go negative
    }
}

bool AFarmGameTaskPlayerController::ServerBuySeeds_Validate(int32 Cost)
{
    // Optionally check if Cost is valid, or if the player can afford it
    return true;
}

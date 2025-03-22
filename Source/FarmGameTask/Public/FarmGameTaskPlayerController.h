// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FarmGameTaskPlayerController.generated.h"

class UFarmBudgetWidget;
class AFarmGameTaskGameState;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class FARMGAMETASK_API AFarmGameTaskPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AFarmGameTaskPlayerController();

	/** 
	 * Shows how to buy seeds from the shared farm budget.
	 * This is just an example function that calls the server to spend money.
	 */
	UFUNCTION(BlueprintCallable, Category="Farm")
	void BuySeeds(int32 SeedCost);
	/** The widget class (Blueprint) we create at runtime to display the budget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UFarmBudgetWidget> FarmBudgetWidgetClass;

	/** The actual instance of the UI widget for this player */
	UPROPERTY()
	UFarmBudgetWidget* FarmBudgetWidgetInstance;

	// Pointer to your UInputAction asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Interact;

	// Pointer to your UInputMappingContext asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* IMC_MyMappings;
protected:

	// Called when the game starts (after this controller is fully initialized).
	virtual void BeginPlay() override;

	// Called to bind input axes and actions. (E.g., Interact)
	virtual void SetupInputComponent() override;

	// Server RPC for buying seeds with a cost.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBuySeeds(int32 Cost);

	/**
	 * Example Interact function called via 'E' key. 
	 * Typically you'd do a line trace to see what you're interacting with, 
	 * then call a server RPC to perform the actual action on the server.
	 */
	void Interact();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();

	//----------------------------------
	// UI References
	//----------------------------------


};

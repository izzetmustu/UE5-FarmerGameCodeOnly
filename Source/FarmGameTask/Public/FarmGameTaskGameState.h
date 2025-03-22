// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FarmGameTaskGameState.generated.h"

/**
 * 
 */
UCLASS()
class FARMGAMETASK_API AFarmGameTaskGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AFarmGameTaskGameState();
	/** 
	 * The shared budget for the farm (replicated).
	 * Adjust via ServerChangeBudget() on the server to keep all clients in sync.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_FarmBudget, BlueprintReadOnly, Category="Farm")
	int32 FarmBudget;
	/**
	 * Server function to change the budget by a given delta (positive or negative).
	 * Example usage: 
	 *   ServerChangeBudget(-SeedCost);   (if buying seeds)
	 *   ServerChangeBudget(+SellPrice);  (if selling produce)
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeBudget(int32 Delta);
	/**
	* Example BlueprintCallable function to read the current budget in Blueprints.
	*/
	UFUNCTION(BlueprintCallable, Category="Farm")
	FORCEINLINE int32 GetFarmBudget() const { return FarmBudget; };
protected:
	// Called once on the server when the game starts or is spawned
	virtual void BeginPlay() override;
	/**
	 * Called on clients whenever FarmBudget has been updated by the server.
	 * You can add client-side visual or audio feedback here.
	 */
	UFUNCTION()
	void OnRep_FarmBudget();
	// --- Replication ---
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

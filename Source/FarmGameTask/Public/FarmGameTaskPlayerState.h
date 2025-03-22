// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FarmGameTaskPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FARMGAMETASK_API AFarmGameTaskPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
    // Constructor
    AFarmGameTaskPlayerState();



    //------------------------------------------------
    // Basic Accessors
    //------------------------------------------------

    // Increments the player's harvested crops (Server RPC)
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerIncrementCropsHarvested(int32 Amount);

    // Increments the player's money spent (Server RPC)
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerIncrementMoneySpent(int32 Amount);

    // Increments the player's "FarmScore" (Server RPC)
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerIncrementFarmScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetCropsHarvested() const { return CropsHarvested;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetMoneySpent() const { return MoneySpent;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetFarmScore() const { return FarmScore;};
protected:
    //------------------------------------------------
    // Replicated Variables
    //------------------------------------------------

    /** Number of crops this player has harvested (personal count). */
    UPROPERTY(ReplicatedUsing = OnRep_CropsHarvested, BlueprintReadOnly, Category="Farm|Stats")
    int32 CropsHarvested;
    /** Tracks how much money this player personally spent (e.g., on seeds, etc.). */
    UPROPERTY(ReplicatedUsing = OnRep_MoneySpent, BlueprintReadOnly, Category="Farm|Stats")
    int32 MoneySpent;
    /** A custom "score" that might be used for ranking players or awarding achievements. */
    UPROPERTY(ReplicatedUsing = OnRep_FarmScore, BlueprintReadOnly, Category="Farm|Stats")
    int32 FarmScore;
    
    //------------------------------------------------
    // RepNotify Functions
    //------------------------------------------------
    /**
     * Called on clients when CropsHarvested is updated by the server.
     * Great place to trigger UI updates or other client-side logic.
     */
    UFUNCTION()
    void OnRep_CropsHarvested();

    /**
     * Called on clients when MoneySpent is updated by the server.
     */
    UFUNCTION()
    void OnRep_MoneySpent();

    /**
     * Called on clients when FarmScore is updated by the server.
     */
    UFUNCTION()
    void OnRep_FarmScore();

    //------------------------------------------------
    // PlayerState Overrides
    //------------------------------------------------

public:
    /**
     * Overridden so that we can replicate our additional variables.
     */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //------------------------------------------------
    // Additional Logic
    //------------------------------------------------

    // You can add more advanced logic here, like saving/loading data, synergy with the farm GameState, etc.

};

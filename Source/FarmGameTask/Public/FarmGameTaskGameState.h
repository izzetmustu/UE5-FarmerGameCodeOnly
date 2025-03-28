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

	UPROPERTY(ReplicatedUsing=OnRep_FarmBudget, BlueprintReadOnly, Category="Farm")
	int32 FarmBudget;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeBudget(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Farm")
	FORCEINLINE int32 GetFarmBudget() const { return FarmBudget; };
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_FarmBudget();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

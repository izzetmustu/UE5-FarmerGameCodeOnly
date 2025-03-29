// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

class ASalesCounter;

UCLASS()
class FARMGAMETASK_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCController();

protected:
	FTimerHandle SalesCheckTimerHandle;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UPROPERTY(EditAnywhere, Category="NPC|Roaming")
	TArray<AActor*> Waypoints;
	
	UPROPERTY(EditAnywhere, Category="NPC|Sales")
	ASalesCounter* SalesCounterRef;

	UFUNCTION()
	void CheckSalesItems();

	UPROPERTY()
	bool bGoingToBuy;
	UPROPERTY()
	bool bWandering;
	
	void GoToRandomWaypoint();
	void GoToSalesCounter();

	void BuyItems();
};

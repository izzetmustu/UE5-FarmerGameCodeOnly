// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmGameTask/Public/CropTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "SalesCounter.generated.h"

class AFarmGameTaskGameState;

UCLASS()
class FARMGAMETASK_API ASalesCounter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASalesCounter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SalesCounter")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SalesCounter|Visual")
	UMaterialInterface* CounterMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SalesCounter|Collision")
	UBoxComponent* OverlapBox;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(ReplicatedUsing=OnRep_WheatCount, BlueprintReadOnly, Category="Sales")
	int32 WheatCount;
	UPROPERTY(ReplicatedUsing=OnRep_CornCount, BlueprintReadOnly, Category="Sales")
	int32 CornCount;
	UPROPERTY(ReplicatedUsing=OnRep_ProcessedWheat, BlueprintReadOnly, Category="Sales")
	int32 ProcessedWheatCount;
	UPROPERTY(ReplicatedUsing=OnRep_ProcessedCorn, BlueprintReadOnly, Category="Sales")
	int32 ProcessedCornCount;
	
	UFUNCTION()
	void OnRep_WheatCount();
	UFUNCTION()
	void OnRep_CornCount();
	UFUNCTION()
	void OnRep_ProcessedWheat();
	UFUNCTION()
	void OnRep_ProcessedCorn();

	UFUNCTION(BlueprintCallable, Category="Sales")
	void UpdateDisplayWidget();
public:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Sales")
	void ServerPlaceCrop(ECropType CropType, int32 Amount);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Sales")
	void ServerBuyCrop(ECropType CropType, int32 Amount);
	
	UFUNCTION()
	void OnOverlapBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						   bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable, Category="Sales")
	int32 GetWheatCount() const { return WheatCount; }
	UFUNCTION(BlueprintCallable, Category="Sales")
	int32 GetCornCount() const { return CornCount; }
	UFUNCTION(BlueprintCallable, Category="Sales")
	int32 GetProcessedWheatCount() const { return ProcessedWheatCount; }
	UFUNCTION(BlueprintCallable, Category="Sales")
	int32 GetProcessedCornCount() const { return ProcessedCornCount; }

	AFarmGameTaskGameState* GetFarmGameState() const;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

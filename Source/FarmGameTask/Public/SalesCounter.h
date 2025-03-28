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
	ASalesCounter();

protected:
	virtual void BeginPlay() override;

	TMap<ECropType, int32> CropPrices;
	TMap<ECropType, int32> ProcessedCropPrices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SalesCounter")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SalesCounter|Visual")
	UMaterialInterface* CounterMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SalesCounter|Collision")
	UBoxComponent* OverlapBox;
public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Sales")
	int32 WheatCount;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Sales")
	int32 CornCount;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Sales")
	int32 ProcessedWheatCount;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Sales")
	int32 ProcessedCornCount;

public:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Sales")
	void ServerChangeStock(ECropType CropType, int32 Amount);
	
	UFUNCTION()
	void OnOverlapBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						   bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable, Category="Sales")
	int32 GetCropCount(ECropType CropType) const;
	UFUNCTION(Blueprintable, Category="Sales")
	FORCEINLINE int32 GetCropPrice(ECropType CropType) const { return CropPrices.FindRef(CropType); }
	UFUNCTION(BlueprintCallable, Category="Sales")
	FORCEINLINE int32 GetProcessedWheatCount() const { return ProcessedWheatCount; }
	UFUNCTION(BlueprintCallable, Category="Sales")
	FORCEINLINE int32 GetProcessedCornCount() const { return ProcessedCornCount; }

	
	AFarmGameTaskGameState* GetFarmGameState() const;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

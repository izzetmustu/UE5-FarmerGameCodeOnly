// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotTypes.h"
#include "CropSlot.generated.h"

UCLASS()
class FARMGAMETASK_API ACropSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	ACropSlot();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(ReplicatedUsing = OnRep_SlotInfo, BlueprintReadWrite, EditAnywhere)
	FSlotInfo SlotInfo;

	FTimerHandle GrowthTimerHandle;
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* OverlapBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMaterial;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void SetSlotInfo(FSlotInfo InSlotInfo);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FSlotInfo GetSlotInfo() const { return SlotInfo; }
	
	UFUNCTION()
	void ClearTimer();
	UFUNCTION()
	void StartTimer();
	UFUNCTION()
	void OnGrowthTick();
	UFUNCTION()
	void UpdateVisuals();
	UFUNCTION()
	void OnRep_SlotInfo();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

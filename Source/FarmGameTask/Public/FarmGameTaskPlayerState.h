#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FarmGameTask/Public/CropTypes.h"
#include "FarmGameTaskPlayerState.generated.h"

UCLASS()
class FARMGAMETASK_API AFarmGameTaskPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
    AFarmGameTaskPlayerState();
    
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerIncrementCropsHarvested(int32 Amount);
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerIncrementMoneySpent(int32 Amount);
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerIncrementCropsProcessed(int32 Amount);
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerAddToInventory(ECropType ItemType, int32 Amount);

protected:
    UPROPERTY(ReplicatedUsing=OnRep_Inventory, BlueprintReadOnly, Category="Inventory")
    int32 CornCount;
    UPROPERTY(ReplicatedUsing=OnRep_Inventory, BlueprintReadOnly, Category="Inventory")
    int32 WheatCount;
    UPROPERTY(ReplicatedUsing=OnRep_Inventory, BlueprintReadOnly, Category="Inventory")
    int32 ProcessedCornCount;
    UPROPERTY(ReplicatedUsing=OnRep_Inventory, BlueprintReadOnly, Category="Inventory")
    int32 ProcessedWheatCount;
    
    UPROPERTY(ReplicatedUsing = OnRep_MoneySpent, BlueprintReadOnly, Category="Farm|Stats")
    int32 MoneySpent;
    UPROPERTY(ReplicatedUsing = OnRep_CropsHarvested, BlueprintReadOnly, Category="Farm|Stats")
    int32 CropsHarvested;
    UPROPERTY(ReplicatedUsing = OnRep_CropsProcessed, BlueprintReadOnly, Category="Farm|Stats")
    int32 CropsProcessed;
    
    UFUNCTION()
    void OnRep_CropsHarvested();
    
    UFUNCTION()
    void OnRep_MoneySpent();
    
    UFUNCTION()
    void OnRep_CropsProcessed();
    
    UFUNCTION()
    void OnRep_Inventory();
    

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Sales")
    int32 GetCropCount(ECropType CropType) const;
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetCropsHarvested() const { return CropsHarvested;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetMoneySpent() const { return MoneySpent;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetCropsProcessed() const { return CropsProcessed;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetCornCount() const { return CornCount;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetWheatCount() const { return WheatCount;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetProcessedCornCount() const { return ProcessedCornCount;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetProcessedWheatCount() const { return ProcessedWheatCount;};
};



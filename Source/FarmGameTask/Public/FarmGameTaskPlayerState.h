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
    void ServerMoneySpent(int32 Amount);
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Farm|Stats")
    void ServerAddToInventory(ECropType ItemType, int32 Amount);
    
protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
    int32 CornCount;
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
    int32 WheatCount;
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
    int32 HarvestedCornCount;
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
    int32 HarvestedWheatCount;
    UPROPERTY(ReplicatedUsing = OnRep_MoneySpent, BlueprintReadOnly, Category="Farm|Stats")
    int32 MoneySpent;
    UPROPERTY(ReplicatedUsing = OnRep_Inventory, BlueprintReadOnly, Category="Farm|Stats")
    int32 CropsHarvested;
    
    UFUNCTION()
    void OnRep_MoneySpent();
    UFUNCTION()
    void OnRep_Inventory();   

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Sales")
    int32 GetCropCount(ECropType CropType) const;
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetMoneySpent() const { return MoneySpent;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetCropsHarvested() const { return CropsHarvested;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetCornCount() const { return CornCount;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetWheatCount() const { return WheatCount;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetHarvestedCornCount() const { return HarvestedCornCount;};
    UFUNCTION(BlueprintCallable, Category="Farm|Stats")
    FORCEINLINE int32 GetHarvestedWheatCount() const { return HarvestedWheatCount;};
};



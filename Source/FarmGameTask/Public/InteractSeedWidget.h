// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractSeedWidget.generated.h"

class UButton;
class ACropSlot;

UCLASS()
class FARMGAMETASK_API UInteractSeedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCropSlot(ACropSlot *InSlot);
	UFUNCTION(BlueprintCallable)
	void EnableWidgetInteractable();
	UFUNCTION(BlueprintCallable)
	void DisableWidgetInteractable();
	
protected:
	UPROPERTY()
	ACropSlot* LinkedSlot;
	UPROPERTY()
	int32 LinkedSlotIndex;

	UPROPERTY(meta=(BindWidget))
	UButton* WheatButton;
	UPROPERTY(meta=(BindWidget))
	UButton* CornButton;
	UPROPERTY(meta=(BindWidget))
	UButton* HarvestButton;
	
	UFUNCTION()
	void OnWheatClicked();
	UFUNCTION()
	void OnCornClicked();
	UFUNCTION()
	void OnHarvestClicked();


	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateButtonVisibility();
};

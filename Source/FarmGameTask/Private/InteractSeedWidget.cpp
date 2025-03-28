#include "InteractSeedWidget.h"
// #include "FarmGameTask/Public/CropField.h"
#include "FarmGameTaskPlayerState.h"
#include "FarmGameTask/Public/CropSlot.h"
#include "FarmGameTask/Public/SlotTypes.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "FarmGameTask/Public/FarmGameTaskPlayerController.h"

void UInteractSeedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WheatButton)
	{
		WheatButton->OnClicked.AddDynamic(this, &UInteractSeedWidget::OnWheatClicked);
	}
	if (CornButton)
	{
		CornButton->OnClicked.AddDynamic(this, &UInteractSeedWidget::OnCornClicked);
	}
	if (HarvestButton)
	{
		HarvestButton->OnClicked.AddDynamic(this, &UInteractSeedWidget::OnHarvestClicked);
	}
	UpdateButtonVisibility();
}

void UInteractSeedWidget::SetCropSlot(ACropSlot *InSlot)
{
	LinkedSlot = InSlot;
	UpdateButtonVisibility();
}

void UInteractSeedWidget::EnableWidgetInteractable()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->DisableInput(PC);
		}
	}
}

void UInteractSeedWidget::DisableWidgetInteractable()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->EnableInput(PC);
		}
	}
}

void UInteractSeedWidget::UpdateButtonVisibility()
{
	if (!LinkedSlot)
	{
		if (WheatButton)   WheatButton->SetVisibility(ESlateVisibility::Collapsed);
		if (CornButton)    CornButton->SetVisibility(ESlateVisibility::Collapsed);
		if (HarvestButton) HarvestButton->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	const FSlotInfo SlotInfo= LinkedSlot->GetSlotInfo();
	switch (SlotInfo.State)
	{
	case ESlotState::Empty:
		{
			if (WheatButton)   WheatButton->SetVisibility(ESlateVisibility::Visible);
			if (CornButton)    CornButton->SetVisibility(ESlateVisibility::Visible);
			if (HarvestButton) HarvestButton->SetVisibility(ESlateVisibility::Collapsed);
			if (WheatOwned)    WheatOwned->SetVisibility(ESlateVisibility::Visible);
			if (CornOwned)    CornOwned->SetVisibility(ESlateVisibility::Visible);
			CornOwned->SetText((FText::AsNumber(GetOwningPlayer()->GetPlayerState<AFarmGameTaskPlayerState>()->GetCornCount())));
			WheatOwned->SetText((FText::AsNumber(GetOwningPlayer()->GetPlayerState<AFarmGameTaskPlayerState>()->GetWheatCount())));
			break;
		}
	case ESlotState::Growing:
		{
			if (WheatButton)   WheatButton->SetVisibility(ESlateVisibility::Collapsed);
			if (CornButton)    CornButton->SetVisibility(ESlateVisibility::Collapsed);
			if (HarvestButton) HarvestButton->SetVisibility(ESlateVisibility::Collapsed);
			if (WheatOwned)    WheatOwned->SetVisibility(ESlateVisibility::Collapsed);
			if (CornOwned)    CornOwned->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	case ESlotState::ReadyToHarvest:
		{
			if (WheatButton)   WheatButton->SetVisibility(ESlateVisibility::Collapsed);
			if (CornButton)    CornButton->SetVisibility(ESlateVisibility::Collapsed);
			if (HarvestButton) HarvestButton->SetVisibility(ESlateVisibility::Visible);
			if (WheatOwned)    WheatOwned->SetVisibility(ESlateVisibility::Collapsed);
			if (CornOwned)    CornOwned->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
	case ESlotState::Damaged:
		{
			if (WheatButton)   WheatButton->SetVisibility(ESlateVisibility::Collapsed);
			if (CornButton)    CornButton->SetVisibility(ESlateVisibility::Collapsed);
			if (HarvestButton) HarvestButton->SetVisibility(ESlateVisibility::Visible);
			if (WheatOwned)    WheatOwned->SetVisibility(ESlateVisibility::Collapsed);
			if (CornOwned)    CornOwned->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
	default:
		{
			if (WheatButton)   WheatButton->SetVisibility(ESlateVisibility::Collapsed);
			if (CornButton)    CornButton->SetVisibility(ESlateVisibility::Collapsed);
			if (HarvestButton) HarvestButton->SetVisibility(ESlateVisibility::Collapsed);
			if (WheatOwned)    WheatOwned->SetVisibility(ESlateVisibility::Collapsed);
			if (CornOwned)    CornOwned->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	}
	EnableWidgetInteractable();
}

void UInteractSeedWidget::OnWheatClicked()
{
	if (!LinkedSlot) return;
	
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (AFarmGameTaskPlayerController* MyPC = Cast<AFarmGameTaskPlayerController>(PC))
		{
			if (MyPC->GetPlayerState<AFarmGameTaskPlayerState>()->GetWheatCount() > 0)
			{
				FSlotInfo NewSlotInfo;
				NewSlotInfo.State = ESlotState::Growing;
				NewSlotInfo.SeedType = ESlotSeedType::Wheat;
				NewSlotInfo.TimeRemaining = 10.0f;
				MyPC->ServerAttemptSowSlot(LinkedSlot, NewSlotInfo);
				DisableWidgetInteractable();
				RemoveFromParent();
			}
		}
	}
}

void UInteractSeedWidget::OnCornClicked()
{
	if (!LinkedSlot) return;
	
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (AFarmGameTaskPlayerController* MyPC = Cast<AFarmGameTaskPlayerController>(PC))
		{
			if (MyPC->GetPlayerState<AFarmGameTaskPlayerState>()->GetCornCount() > 0)
			{
				FSlotInfo NewSlotInfo;
				NewSlotInfo.State = ESlotState::Growing;
				NewSlotInfo.SeedType = ESlotSeedType::Corn;
				NewSlotInfo.TimeRemaining = 15.0f;
				MyPC->ServerAttemptSowSlot(LinkedSlot, NewSlotInfo);
				DisableWidgetInteractable();
				RemoveFromParent();
			}
		}
	}
}

void UInteractSeedWidget::OnHarvestClicked()
{
	if (!LinkedSlot) return;
	
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (AFarmGameTaskPlayerController* MyPC = Cast<AFarmGameTaskPlayerController>(PC))
		{
			if (MyPC)
			{
				MyPC->ServerAttemptHarvestSlot(LinkedSlot);
				DisableWidgetInteractable();
				RemoveFromParent();
			}
		}
	}
}


#include "Player/MTD_PlayerController.h"

#include "CommonInputSubsystem.h"
#include "Input/CommonUIActionRouterBase.h"

void AMTH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupFocusHandling();
}

void AMTH_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldResetFocus)
	{
		bShouldResetFocus = false;
		if (WidgetToFocusOn.IsValid())
		{
			FSlateApplication::Get().SetUserFocus(
				GetLocalPlayer()->GetSlateUser()->GetUserIndex(), WidgetToFocusOn, EFocusCause::SetDirectly);
		}
	}
}

void AMTH_PlayerController::SetupFocusHandling()
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return;
	}

	auto* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
	check(IsValid(InputSubsystem));

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnFocusChanging().AddUObject(this, &ThisClass::OnFocusChanged);
	}
}

void AMTH_PlayerController::OnFocusChanged(const FFocusEvent& FocusEvent,
	const FWeakWidgetPath& OldFocusedWidgetPath, const TSharedPtr<SWidget>& OldFocusedWidget,
	const FWidgetPath& NewFocusedWidgetPath, const TSharedPtr<SWidget>& NewFocusedWidget)
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return;
	}

	const int32 ControllerID = LocalPlayer->GetControllerId();
	if (ControllerID != FocusEvent.GetUser())
	{
		return;
	}

	// If we're using menu, we want to keep the focus on anything but viewport;
	// the focus cannot be reset right away, as Slate will ignore a subsequent SetFocus if the widget we're trying to
	// focus on is the one that we're unfocusing from right now, so it's done the next frame on tick;
	// the reason it's done on tick, is because the timer doesn't want to fire for no apparent reason
	auto* ActionRouter = ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(LocalPlayer);
	const bool bIsMenu = ActionRouter->GetActiveInputMode() == ECommonInputMode::Menu;
	const bool bIsViewport = NewFocusedWidget.IsValid() && NewFocusedWidget->ToString().Contains("Viewport");
	if (bIsMenu && bIsViewport)
	{
		bShouldResetFocus = true;
		WidgetToFocusOn = OldFocusedWidget;
	}
}

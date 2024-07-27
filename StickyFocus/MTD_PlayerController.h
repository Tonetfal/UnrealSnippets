#pragma once

#include "GameFramework/PlayerController.h"

#include "MTH_PlayerController.generated.h"

UCLASS()
class AMTH_PlayerController
	: public APlayerController
{
	GENERATED_BODY()

protected:
	//~ArPlayerController Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	//~End of ArPlayerController Interface

private:
	void SetupFocusHandling();

	void OnFocusChanged(const FFocusEvent& FocusEvent,
		const FWeakWidgetPath& OldFocusedWidgetPath, const TSharedPtr<SWidget>& OldFocusedWidget,
		const FWidgetPath& NewFocusedWidgetPath, const TSharedPtr<SWidget>& NewFocusedWidget);

protected:
	bool bShouldResetFocus = false;
	TSharedPtr<SWidget> WidgetToFocusOn = nullptr;
};

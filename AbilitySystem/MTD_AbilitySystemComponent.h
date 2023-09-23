#pragma once

#include "AbilitySystem/LyraAbilitySystemComponent.h"

#include "MTD_AbilitySystemComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(MTD_LogAbilitySystem, Log, All);

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
struct FMTD_GameplayAbilityGenericInputBindings;

/**
 * Default ability system component used in this project.
 */
UCLASS()
class MTD_API UMTD_AbilitySystemComponent
	: public ULyraAbilitySystemComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(
		FOnGenericActionTriggeredSignature,
		FGameplayTag, GenericActionTag);

	DECLARE_DYNAMIC_DELEGATE(FOnSpecificGenericActionTriggeredSignature);

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
		FOnGenericActionTriggeredMulticastSignature,
		FGameplayTag, GenericActionTag);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpecificGenericActionTriggeredMulticastSignature);

public:
	UMTD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	//~ULyraAbilitySystemComponent Interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~End of ULyraAbilitySystemComponent Interface

	/**
	 * Listen for any generic input action.
	 * @param	Event delegate to fire anytime the input triggers.
	 */
	UFUNCTION(BlueprintCallable)
	void BindGenericEvent(FOnGenericActionTriggeredSignature Event);

	/**
	 * Stop listening for generic input actions.
	 * @param	Event delegate to unbind.
	 */
	UFUNCTION(BlueprintCallable)
	void UnbindGenericEvent(FOnGenericActionTriggeredSignature Event);

	/**
	 * Listen for a specific generic input action.
	 * @param	Event delegate to fire anytime the input triggers.
	 * @param	GenericActionTag specific generic action to listen.
	 */
	UFUNCTION(BlueprintCallable)
	void BindSpecificGenericEvent(FOnSpecificGenericActionTriggeredSignature Event, FGameplayTag GenericActionTag);

	/**
	 * Stop listening for a specific generic input actions.
	 * @param	Event delegate to unbind.
	 * @param	GenericActionTag specific generic action to stop listening.
	 */
	UFUNCTION(BlueprintCallable)
	void UnbindSpecificGenericEvent(FOnSpecificGenericActionTriggeredSignature Event, FGameplayTag GenericActionTag);

protected:
	/**
	 * Called when any generic input action from UMTD_AbilitySystemSettings::GenericInputMappingContext gets
	 * triggered.
	 * @param	GenericActionTag gameplay tag associated with the input action.
	 */
	virtual void OnGenericActionInput(FGameplayTag GenericActionTag);

private:
	/**
	 * Listen for all given input actions trigger events.
	 * @param	InputComponent input component to listen input actions on.
	 * @param	InputBindings bindings data to bind.
	 */
	void BindGenericActionsToInputComponent(UInputComponent* InputComponent,
		const FMTD_GameplayAbilityGenericInputBindings& InputBindings);

	/**
	 * Get enhanced input local player subsystem.
	 * @return	Enhanced input local player subsystem.
	 */
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem() const;

	/**
	 * Get generic actions input mapping context
	 * @return	Generic actions input mapping context
	 */
	UInputMappingContext* GetGenericActionsInputMappingContext() const;

	/**
	 * Bind or unbind generic actions input mapping context.
	 * @param	bBindInputs if true bind inputs, unbind otherwise.
	 */
	void ChangeGenericActionsMappingBindingState(bool bBindInputs) const;

	/**
	 * Check whether there is any generic action currently bound.
	 * @return	If true, at least one delegate is bound, false if none.
	 */
	bool IsAnyGenericActionDelegateBound() const;

private:
	/** Fired anytime generic input action gets triggered. */
	FOnGenericActionTriggeredMulticastSignature OnGenericActionTriggeredDelegate;

	TMap<FGameplayTag, FOnSpecificGenericActionTriggeredMulticastSignature> OnSpecificGenericActionTriggeredDelegates;
};


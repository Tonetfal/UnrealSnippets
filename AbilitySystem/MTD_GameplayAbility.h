#pragma once

#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

#include "MTD_GameplayAbility.generated.h"

/**
 * Default gameplay ability used in this project.
 */
UCLASS()
class MTD_API UMTD_GameplayAbility
	: public ULyraGameplayAbility
{
	GENERATED_BODY()

public:
	UMTD_GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ULyraGameplayAbility Interface
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	virtual void K2_EndAbility() override;
	//~End of ULyraGameplayAbility Interface

protected:
	/**
	 * Called when any generic action triggers.
	 * @param	GenericActionTag gameplay tag associated with generic action.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnGenericActionTriggered(FGameplayTag GenericActionTag);

	/**
	 * Called when confirm action triggers.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnConfirmActionTriggered();

	/**
	 * Called when cancel action triggers.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCancelActionTriggered();

private:
	/**
	 * Bind or unbind generic action event.
	 * @param  bBind bind if true, unbind otherwise.
	 */
	void ChangeGenericActionBindingState(bool bBind);

protected:
	/**
	 * If true, all generic action events will be bound and unbound automatically on ability start and end, false
	 * otherwise.
	 * @see		UMTD_GameplayAbility::ChangeGenericActionBindingState()
	 */
	UPROPERTY(EditDefaultsOnly, Category="MTD|Ability Inputs")
	bool bActivateGenericInputActions = false;
};


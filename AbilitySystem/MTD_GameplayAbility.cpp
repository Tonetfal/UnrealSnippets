#include "AbilitySystem/Abilities/MTD_GameplayAbility.h"

#include "AbilitySystem/MTD_AbilitySystemComponent.h"
#include "MTD_GameplayTags.h"

UMTD_GameplayAbility::UMTD_GameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMTD_GameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled())
	{
		ChangeGenericActionBindingState(true);
	}
}

void UMTD_GameplayAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (IsLocallyControlled())
	{
		ChangeGenericActionBindingState(false);
	}
}

void UMTD_GameplayAbility::K2_EndAbility()
{
	Super::K2_EndAbility();

	// Move the method from protected to public
}

void UMTD_GameplayAbility::OnGenericActionTriggered_Implementation(FGameplayTag GenericActionTag)
{
	// Empty
}

void UMTD_GameplayAbility::OnConfirmActionTriggered_Implementation()
{
	// Empty
}

void UMTD_GameplayAbility::OnCancelActionTriggered_Implementation()
{
	// Empty
}

void UMTD_GameplayAbility::ChangeGenericActionBindingState(bool bBind)
{
	if (bActivateGenericInputActions)
	{
		auto* AbilitySystemComponent = GetTypedAbilitySystemComponentFromActorInfo<UMTD_AbilitySystemComponent>();
		if (IsValid(AbilitySystemComponent) && IsValid(this))
		{
			UMTD_AbilitySystemComponent::FOnGenericActionTriggeredSignature GenericDelegate;
			GenericDelegate.BindDynamic(this, &ThisClass::OnGenericActionTriggered);

			UMTD_AbilitySystemComponent::FOnSpecificGenericActionTriggeredSignature ConfirmDelegate;
			ConfirmDelegate.BindDynamic(this, &ThisClass::OnConfirmActionTriggered);

			UMTD_AbilitySystemComponent::FOnSpecificGenericActionTriggeredSignature CancelDelegate;
			CancelDelegate.BindDynamic(this, &ThisClass::OnCancelActionTriggered);

			if (bBind)
			{
				AbilitySystemComponent->BindGenericEvent(GenericDelegate);
				AbilitySystemComponent->BindSpecificGenericEvent(ConfirmDelegate, MTD_GameplayTags::InputTag_Confirm);
				AbilitySystemComponent->BindSpecificGenericEvent(CancelDelegate, MTD_GameplayTags::InputTag_Cancel);
			}
			else
			{
				AbilitySystemComponent->UnbindGenericEvent(GenericDelegate);
				AbilitySystemComponent->UnbindSpecificGenericEvent(ConfirmDelegate, MTD_GameplayTags::InputTag_Confirm);
				AbilitySystemComponent->UnbindSpecificGenericEvent(CancelDelegate, MTD_GameplayTags::InputTag_Cancel);
			}
		}
	}
}


#include "AbilitySystem/MTD_AbilitySystemComponent.h"

#include "AbilitySystem/MTD_AbilitySystemInputs.h"
#include "AbilitySystem/MTD_AbilitySystemSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Utilities/MTD_Log.h"

DEFINE_LOG_CATEGORY(MTD_LogAbilitySystem);

UMTD_AbilitySystemComponent::UMTD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMTD_AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (IsValid(InAvatarActor))
	{
		auto* InputComponent = InAvatarActor->FindComponentByClass<UInputComponent>();
		if (IsValid(InputComponent))
		{
			const auto* SettingsCDO = GetDefault<UMTD_AbilitySystemSettings>();
			BindGenericActionsToInputComponent(InputComponent, SettingsCDO->GenericInputs);
		}
	}
}

void UMTD_AbilitySystemComponent::BindGenericEvent(FOnGenericActionTriggeredSignature Event)
{
	TScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(Event.GetUObject(), Event.GetFunctionName());

	const bool bWasBound = IsAnyGenericActionDelegateBound();
	OnGenericActionTriggeredDelegate.Add(ScriptDelegate);

	if (!bWasBound)
	{
		ChangeGenericActionsMappingBindingState(true);
	}
}

void UMTD_AbilitySystemComponent::UnbindGenericEvent(FOnGenericActionTriggeredSignature Event)
{
	TScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(Event.GetUObject(), Event.GetFunctionName());

	OnGenericActionTriggeredDelegate.Remove(ScriptDelegate);

	if (!IsAnyGenericActionDelegateBound())
	{
		ChangeGenericActionsMappingBindingState(false);
	}
}

void UMTD_AbilitySystemComponent::BindSpecificGenericEvent(FOnSpecificGenericActionTriggeredSignature Event,
	FGameplayTag GenericActionTag)
{
	TScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(Event.GetUObject(), Event.GetFunctionName());

	const bool bWasBound = IsAnyGenericActionDelegateBound();
	FOnSpecificGenericActionTriggeredMulticastSignature& MulticastDelegate =
		OnSpecificGenericActionTriggeredDelegates.FindOrAdd(GenericActionTag);
	MulticastDelegate.Add(ScriptDelegate);

	if (!bWasBound)
	{
		ChangeGenericActionsMappingBindingState(true);
	}
}

void UMTD_AbilitySystemComponent::UnbindSpecificGenericEvent(FOnSpecificGenericActionTriggeredSignature Event,
	FGameplayTag GenericActionTag)
{
	TScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(Event.GetUObject(), Event.GetFunctionName());

	FOnSpecificGenericActionTriggeredMulticastSignature* MulticastDelegate =
		OnSpecificGenericActionTriggeredDelegates.Find(GenericActionTag);
	if (!ensureMsgf(MulticastDelegate, TEXT("Trying to remove event [%s] [%s] for [%s] which is not bound."),
		*GetNameSafe(Event.GetUObject()), *Event.GetFunctionName().ToString(), *GenericActionTag.ToString()))
	{
		return;
	}

	MulticastDelegate->Remove(ScriptDelegate);
	if (!MulticastDelegate->IsBound())
	{
		OnSpecificGenericActionTriggeredDelegates.Remove(GenericActionTag);
	}

	if (!IsAnyGenericActionDelegateBound())
	{
		ChangeGenericActionsMappingBindingState(false);
	}
}

void UMTD_AbilitySystemComponent::OnGenericActionInput(FGameplayTag GenericActionTag)
{
	MTD_LOG(MTD_LogAbilitySystem, "Generic action [%s] has been triggered.", *GenericActionTag.ToString());

	OnGenericActionTriggeredDelegate.Broadcast(GenericActionTag);

	const FOnSpecificGenericActionTriggeredMulticastSignature* MulticastDelegate =
		OnSpecificGenericActionTriggeredDelegates.Find(GenericActionTag);
	if (MulticastDelegate)
	{
		MulticastDelegate->Broadcast();
	}
}

void UMTD_AbilitySystemComponent::BindGenericActionsToInputComponent(UInputComponent* InputComponent,
	const FMTD_GameplayAbilityGenericInputBindings& InputBindings)
{
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		MTD_ERROR(MTD_LogAbilitySystem, "Input component [%s] is not of type UEnhancedInputComponent.",
			*GetNameSafe(InputComponent))
		return;
	}

	int32 BindingIndex = 0;
	for (const FMTD_GameplayAbilityGenericInputBinding& Binding : InputBindings.Bindings)
	{
		if (!Binding.Tag.IsValid())
		{
			MTD_WARN(MTD_LogAbilitySystem, "Generic action tag for action index [%d] is invalid.",
				BindingIndex, *Binding.Tag.ToString());
			BindingIndex++;

			continue;
		}

		int32 ActionIndex = 0;
		for (TObjectPtr<UInputAction> InputAction : Binding.InputActions)
		{
			if (!InputAction)
			{
				MTD_WARN(MTD_LogAbilitySystem, "Input action index [%d] for [%s] is invalid.",
					ActionIndex++, *Binding.Tag.ToString());
				ActionIndex++;

				continue;
			}

			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started,
				this, &ThisClass::OnGenericActionInput, Binding.Tag);

			ActionIndex++;
		}

		BindingIndex++;
	}
}

UEnhancedInputLocalPlayerSubsystem* UMTD_AbilitySystemComponent::GetEnhancedInputLocalPlayerSubsystem() const
{
	const AActor* Avatar = GetAvatarActor();
	if (!IsValid(Avatar))
	{
		return nullptr;
	}

	const auto* Character = Cast<ACharacter>(Avatar);
	if (!IsValid(Character))
	{
		return nullptr;
	}

	const APlayerController* PlayerController = Character->GetLocalViewingPlayerController();
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(IsValid(LocalPlayer));

	auto* EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	return EnhancedInputSubsystem;
}

UInputMappingContext* UMTD_AbilitySystemComponent::GetGenericActionsInputMappingContext() const
{
	const auto* SettingsCDO = GetDefault<UMTD_AbilitySystemSettings>();
	if (SettingsCDO->GenericInputMappingContext.IsNull())
	{
		return nullptr;
	}

	UInputMappingContext* InputMappingContext = SettingsCDO->GenericInputMappingContext.LoadSynchronous();
	return InputMappingContext;
}

void UMTD_AbilitySystemComponent::ChangeGenericActionsMappingBindingState(bool bBindInputs) const
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputLocalPlayerSubsystem();
	if (!IsValid(Subsystem))
	{
		MTD_ERROR(MTD_LogAbilitySystem, "Failed to retrieve enhanced input local player subsystem.");
		return;
	}

	const UInputMappingContext* InputMappingContext = GetGenericActionsInputMappingContext();
	if (!IsValid(InputMappingContext))
	{
		MTD_ERROR(MTD_LogAbilitySystem, "Failed to retrieve generic actions input mapping context.");
		return;
	}

	if (bBindInputs)
	{
		const auto* SettingsCDO = GetDefault<UMTD_AbilitySystemSettings>();
		Subsystem->AddMappingContext(InputMappingContext, SettingsCDO->InputPriority);

		MTD_LOG(MTD_LogAbilitySystem, "Generic actions input mapping context has been bound.");
	}
	else
	{
		Subsystem->RemoveMappingContext(InputMappingContext);

		MTD_LOG(MTD_LogAbilitySystem, "Generic actions input mapping context has been unbound.");
	}
}

bool UMTD_AbilitySystemComponent::IsAnyGenericActionDelegateBound() const
{
	return OnGenericActionTriggeredDelegate.IsBound() || !OnSpecificGenericActionTriggeredDelegates.IsEmpty();
}


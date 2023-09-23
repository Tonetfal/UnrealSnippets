#pragma once

#include "GameplayTagContainer.h"

#include "MTD_AbilitySystemInputs.generated.h"

class UInputAction;

/**
 * One single generic input action.
 */
USTRUCT()
struct FMTD_GameplayAbilityGenericInputBinding
{
	GENERATED_BODY()

public:
	/** Input actions associated with the generic input action. */
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UInputAction>> InputActions;

	/** Gameplay tag associated with the generic input action. */
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
};

/**
 * Set of generic input actions.
 */
USTRUCT(BlueprintType)
struct FMTD_GameplayAbilityGenericInputBindings
{
	GENERATED_BODY()

public:
	/** Generic input actions. */
	UPROPERTY(EditAnywhere)
	TArray<FMTD_GameplayAbilityGenericInputBinding> Bindings;
};


#pragma once

#include "AbilitySystem/MTD_AbilitySystemInputs.h"
#include "Engine/DeveloperSettings.h"

#include "MTD_AbilitySystemSettings.generated.h"

class UInputMappingContext;

/**
 * Gameplay ability system settings.
 */
UCLASS(Config="Engine", DefaultConfig, DisplayName="Ability System Settings")
class UMTD_AbilitySystemSettings
	: public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//~UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
	//~End of UDeveloperSettings Interface

public:
	/** Input mapping context containing all generic actions used in this project. */
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UInputMappingContext> GenericInputMappingContext = nullptr;

	/** Priority to bind the UMTD_AbilitySystemSettings::GenericInputMappingContext with. */
	UPROPERTY(Config, EditAnywhere)
	int32 InputPriority = 1000;

	/** All generic actions used in this project. */
	UPROPERTY(Config, EditAnywhere)
	FMTD_GameplayAbilityGenericInputBindings GenericInputs;
};

inline FName UMTD_AbilitySystemSettings::GetCategoryName() const
{
	return "MTD";
}


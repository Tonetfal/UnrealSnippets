#pragma once

#include "AbilitySystem/Abilities/MTD_GameplayAbility.h"

#include "MTD_GameplayAbility_BuildTower.generated.h"

class AMTD_Tower;
class AMTD_Tower_Ghost;
class UMTD_AbilityTask_LocateTowerPosition;

UENUM()
enum class EMTD_BuildTower_State : uint8
{
    None,
    Aborting,
    Moving,
    Rotating,
    Building
}

UCLASS()
class UMTD_GameplayAbility_BuildTower
	: public UMTD_GameplayAbility
{
	GENERATED_BODY()

public:
	UMTD_GameplayAbility_BuildTower(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UMTD_GameplayAbility Interface
	virtual void OnConfirmActionTriggered_Implementation() override;
	virtual void OnCancelActionTriggered_Implementation() override;
	//~End of UMTD_GameplayAbility Interface

private:
    void ChangeState(EMTD_BuildTower_State InState);
    void StartMoving();
    void StartRotating();
    void StartBuilding();

private:
    EMTD_BuildTower_State BuildingState = EMTD_BuildTower_State::None;
};

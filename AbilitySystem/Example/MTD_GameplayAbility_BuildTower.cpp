#include "BuildingSystem/Abilities/MTD_GameplayAbility_BuildTower.h"

UMTD_GameplayAbility_BuildTower::UMTD_GameplayAbility_BuildTower(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bActivateGenericInputActions = true;
}

void UMTD_GameplayAbility_BuildTower::OnConfirmActionTriggered_Implementation()
{
	Super::OnConfirmActionTriggered_Implementation();

    switch (BuildingState)
    {
        // Switch to next state
        case EMTD_BuildTower_State::Moving: ChangeState(EMTD_BuildTower_State::Rotating); break;
        case EMTD_BuildTower_State::Rotating: ChangeState(EMTD_BuildTower_State::Building); break;

        // Ignore
        default: break;
    }
}

void UMTD_GameplayAbility_BuildTower::OnCancelActionTriggered_Implementation()
{
	Super::OnCancelActionTriggered_Implementation();

    switch (BuildingState)
    {
        // Return to previous state
        case EMTD_BuildTower_State::Rotating: ChangeState(EMTD_BuildTower_State::Moving); break;
        case EMTD_BuildTower_State::Building: ChangeState(EMTD_BuildTower_State::Rotating); break;

        // Abort
        default: ChangeState(EMTD_BuildTower_State::Aborting); break;
    }
}

void UMTD_GameplayAbility_BuildTower::ChangedState(EMTD_BuildTower_State InState)
{
    if (BuildingState == InState)
    {
        return;
    }

    BuildingState = InState;
    switch (BuildingState)
    {
        case EMTD_BuildTower_State::Moving: StartMoving(); break;
        case EMTD_BuildTower_State::Rotating: StartRotating(); break;
        case EMTD_BuildTower_State::Building: StartBuilding(); break;
        default: K2_EndAbility(); break;
    }
}

void UMTD_GameplayAbility_BuildTower::StartMoving()
{
    // *your implementation*
}

void UMTD_GameplayAbility_BuildTower::StartRotating()
{
    // *your implementation*
}

void UMTD_GameplayAbility_BuildTower::StartBuilding()
{
    // *your implementation*
}


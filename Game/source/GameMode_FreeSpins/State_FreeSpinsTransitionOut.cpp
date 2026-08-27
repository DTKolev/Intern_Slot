#include "GameModes/Mode_FreeSpins.hpp"
#include "GameModes/Mode_Regular.hpp"
#include "GameObjects/CommonManager.hpp"
#include "MasterStates/MasterState_DoorTransition.hpp"
#include <string>

void FreeSpinsTransitionOut::OnEntry(single::Engine& eng) {
 
    MasterDoorTransition::OnEntry(eng);
    
    CommonManager& common_manager = CommonManager::GetInstance();
    
    title = eng.CreateText("TOTAL CREDITS WON", 64.0f, eng.RandomColor());
    winnings = eng.CreateText(std::to_string(common_manager.free_spins_winnings), 64.0f, eng.RandomColor());
}

void FreeSpinsTransitionOut::Update(single::Engine& eng, double delta_t) {

    MasterDoorTransition::Update(eng, delta_t);

    CommonManager& common_manager = CommonManager::GetInstance();

    if (change_state) {
        common_manager.credits += common_manager.free_spins_winnings;
        eng.StateChange<Betting>();
    }
    if (transition_finished) eng.StopOverlay();
}

void FreeSpinsTransitionOut::Render(single::Engine& eng) {

    MasterDoorTransition::Render(eng);

    if (doors_closed) {

        float title_x = 500.0f - (title.GetWidth() / 2.0f);
        float winnings_x = 500.0f - (winnings.GetWidth() / 2.0f);

        eng.RenderText(title, title_x, 300.0f);
        eng.RenderText(winnings, winnings_x, 310.0f + title.GetHeight());
    }
}

void FreeSpinsTransitionOut::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();
    common_manager.free_spins_winnings = 0;
}
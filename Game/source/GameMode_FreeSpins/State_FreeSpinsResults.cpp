#include "GameModes/Mode_FreeSpins.hpp"
#include <string>

void FreeSpinsResults::OnEntry(const single::Engine& eng) {

    MasterResults::OnEntry(eng);

    win_amount *= 3;

    if (common_manager.free_spins == 0) common_manager.free_spins_mode = false;

    winnings = vis.CreateText("Free Spin Winnings: " + std::to_string(common_manager.free_spins_winnings), 32.0f);
    free_spins = vis.CreateText("Free Spins Remaining: " + std::to_string(common_manager.free_spins), 32.0f, (single::Color){0, 255, 0, 255});
}

void FreeSpinsResults::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    MasterResults::HandleInput(eng, input_event);

    if (input_manager.IsReleased(Key::enter)) {
        if (common_manager.free_spins_mode) eng.StateChange<FreeSpinsReeling>();
        else {
            common_manager.free_spins_winnings += win_amount;
            eng.AddOverlayState<FreeSpinsTransitionOut>();
        }
    }
}

void FreeSpinsResults::Render() {

    MasterResults::Render();

    vis.RenderText(winnings, 50.0f, 610.0f);
    vis.RenderText(free_spins, 260.0f, 650.0f);
}

void FreeSpinsResults::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.free_spins_winnings += win_amount;
}
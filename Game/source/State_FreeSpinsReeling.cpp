#include "../headers/Mode_FreeSpins.hpp"
#include "../headers/CommonManager.hpp"
#include "../headers/InputManager.hpp"
#include <string>

void FreeSpinsReeling::OnEntry(single::Engine& eng) {

    MasterReeling::OnEntry(eng);

    common_manager.free_spins--;

    winnings = eng.CreateText("Free Spin Winnings: " + std::to_string(common_manager.free_spins_winnings), 32.0f);
    free_spins = eng.CreateText("Free Spins Remaining: " + std::to_string(common_manager.free_spins), 32.0f, (single::Color){0, 255, 0, 255});
}

void FreeSpinsReeling::Update(single::Engine& eng, double delta_t) {

    MasterReeling::Update(eng, delta_t);

    if (common_manager.GetGrid().ReelingFinished()) eng.StateChange<FreeSpinsResults>();
}

void FreeSpinsReeling::Render(single::Engine& eng) {

    MasterReeling::Render(eng);

    eng.RenderText(winnings, 50.0f, 610.0f);
    eng.RenderText(free_spins, 260.0f, 650.0f);
}
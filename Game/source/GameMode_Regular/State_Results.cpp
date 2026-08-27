#include "GameModes/Mode_Regular.hpp"
#include "GameModes/Mode_FreeSpins.hpp"
#include "GameModes/Mode_ExtraReel.hpp"

void Results::OnEntry(single::Engine& eng) {

    MasterResults::OnEntry(eng);

    if (common_manager.GetGrid().ScatterAmount() == 2) {
        scatters_found = true;
        common_manager.extra_reel_mode = true;
    }

    credits = eng.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    title = eng.CreateText("RESULTS", 32.0);
    free_spins = eng.CreateText("FREE SPINS MODE ACTIVATED", 32.0f, (single::Color){0, 255, 0, 255});
    extra_reel = eng.CreateText("EXTRA REEL", 32.0f, (single::Color){0, 255, 0, 255});
}

void Results::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    MasterResults::HandleInput(eng, input_event);

    if (input_manager.IsReleased(Key::enter)) {
        if (common_manager.free_spins_mode) eng.OverlayState<FreeSpinsTransitionIn>();
        else if (common_manager.extra_reel_mode) eng.StateChange<ExtraReelTransitionIn>();
        else eng.StateChange<Reeling>();
    }
    if (input_manager.IsReleased(Key::shift)) {
        if (!common_manager.free_spins_mode) eng.StateChange<Betting>();
    }
    if (input_manager.IsReleased(Key::escape)) {
        eng.StateChange<MainMenu>();
    }
}

void Results::Render(single::Engine& eng) {

    MasterResults::Render(eng);

    eng.RenderText(credits, 50.0, 610.0);
    eng.RenderText(title, 260.0, 690.0);

    if (common_manager.free_spins_mode) eng.RenderText(free_spins, 260.0f, 650.0f);
    if (common_manager.extra_reel_mode) eng.RenderText(extra_reel, 260.0f, 650.0f);
}
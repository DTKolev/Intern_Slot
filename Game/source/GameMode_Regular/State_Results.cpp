#include "GameModes/Mode_Regular.hpp"
#include "GameModes/Mode_FreeSpins.hpp"
#include "GameModes/Mode_ExtraReel.hpp"

void Results::OnEntry(const single::Engine& eng) {

    MasterResults::OnEntry(eng);

    if (common_manager.GetGrid().ScatterAmount() == 2) {
        scatters_found = true;
        common_manager.extra_reel_mode = true;
    }

    credits = vis.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    title = vis.CreateText("RESULTS", 32.0);
    free_spins = vis.CreateText("FREE SPINS MODE ACTIVATED", 32.0f, (single::Color){0, 255, 0, 255});
    extra_reel = vis.CreateText("EXTRA REEL", 32.0f, (single::Color){0, 255, 0, 255});
}

void Results::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    MasterResults::HandleInput(eng, input_event);

    if (input_manager.IsReleased(Key::enter)) {
        if (common_manager.free_spins_mode) eng.AddOverlayState<FreeSpinsTransitionIn>();
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

void Results::Render() {

    MasterResults::Render();

    vis.RenderText(credits, 50.0, 610.0);
    vis.RenderText(title, 260.0, 690.0);

    if (common_manager.free_spins_mode) vis.RenderText(free_spins, 260.0f, 650.0f);
    if (common_manager.extra_reel_mode) vis.RenderText(extra_reel, 260.0f, 650.0f);
}
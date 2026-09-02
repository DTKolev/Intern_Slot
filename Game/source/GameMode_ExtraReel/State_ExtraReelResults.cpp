#include "GameModes/Mode_ExtraReel.hpp"
#include <string>

void ExtraReelResults::OnEntry(const single::Engine& eng) {

    MasterResults::OnEntry(eng);

    if (scatters_found) outcome = vis.CreateText("You found the third scatter!", 32.0f, (single::Color){0, 255, 0, 255});
    else outcome = vis.CreateText("No scatter found!", 32.0f, (single::Color){255, 255, 255, 255});
}

void ExtraReelResults::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    MasterResults::HandleInput(eng, input_event);

    if (input_manager.IsReleased(Key::enter)) {
        eng.StateChange<ExtraReelTransitionOut>();
    }
}

void ExtraReelResults::Render() {

    MasterResults::Render();

    vis.RenderText(outcome, 260.0f, 650.0f);
}
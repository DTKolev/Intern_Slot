#include "../headers/GameModes/Mode_ExtraReel.hpp"
#include <string>

void ExtraReelResults::OnEntry(single::Engine& eng) {

    MasterResults::OnEntry(eng);

    if (scatters_found) outcome = eng.CreateText("You found the third scatter!", 32.0f, (single::Color){0, 255, 0, 255});
    else outcome = eng.CreateText("No scatter found!", 32.0f, (single::Color){255, 255, 255, 255});
}

void ExtraReelResults::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    MasterResults::HandleInput(eng, input_event);

    if (input_manager.IsReleased(Key::enter)) {
        eng.StateChange<ExtraReelTransitionOut>();
    }
}

void ExtraReelResults::Render(single::Engine& eng) {

    MasterResults::Render(eng);

    eng.RenderText(outcome, 260.0f, 650.0f);
}
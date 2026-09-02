#include "GameModes/Mode_FreeSpins.hpp"
#include "MasterStates/MasterState_DoorTransition.hpp"
#include <string>

void FreeSpinsTransitionIn::OnEntry(single::Engine& eng) {

    MasterDoorTransition::OnEntry(eng);
    title = vis.CreateText("FREE SPINS MODE", 64.0f, vis.RandomColor());
    test = false;
}
 
void FreeSpinsTransitionIn::Update(single::Engine& eng, double delta_t) {

    MasterDoorTransition::Update(eng, delta_t);

    if (change_state && !test) {
        test = true;
    }
    if (change_state) eng.StateChange<FreeSpinsEntry>();
    if (transition_finished) eng.RemoveOverlayState();
}

void FreeSpinsTransitionIn::Render() {

    MasterDoorTransition::Render();

    if (doors_closed) {

        float x_pos = 500.0f - (title.GetWidth() / 2.0f);
        vis.RenderText(title, x_pos, 300.0f);
    }
}
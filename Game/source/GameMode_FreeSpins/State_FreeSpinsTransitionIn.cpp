#include "GameModes/Mode_FreeSpins.hpp"
#include "TestState.hpp"
#include "MasterStates/MasterState_DoorTransition.hpp"
#include <string>

void FreeSpinsTransitionIn::OnEntry(single::Engine& eng) {

    MasterDoorTransition::OnEntry(eng);
    title = eng.CreateText("FREE SPINS MODE", 64.0f, eng.RandomColor());
    test = false;
}
 
void FreeSpinsTransitionIn::Update(single::Engine& eng, double delta_t) {

    MasterDoorTransition::Update(eng, delta_t);

    if (change_state && !test) {
        test = true;
        eng.AddOverlayState<TestState>();
    }
    if (change_state) eng.StateChange<FreeSpinsEntry>();
    if (transition_finished) eng.RemoveOverlayState();
}

void FreeSpinsTransitionIn::Render(single::Engine& eng) {

    MasterDoorTransition::Render(eng);

    if (doors_closed) {

        float x_pos = 500.0f - (title.GetWidth() / 2.0f);
        eng.RenderText(title, x_pos, 300.0f);
    }
}
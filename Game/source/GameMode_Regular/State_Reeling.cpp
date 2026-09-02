#include "GameModes/Mode_Regular.hpp"

void Reeling::OnEntry(single::Engine& eng) {

    MasterReeling::OnEntry(eng);

    common_manager.credits -= common_manager.bet;

    credits = vis.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    title = vis.CreateText("REELING", 32.0);
}

void Reeling::Update(single::Engine& eng, double delta_t) {

    MasterReeling::Update(eng, delta_t);

    if (common_manager.GetGrid().ReelingFinished()) eng.StateChange<Results>();
}

void Reeling::Render() {

    MasterReeling::Render();

    vis.RenderText(credits, 50.0, 610.0);
    vis.RenderText(title, 260.0, 690.0);
}
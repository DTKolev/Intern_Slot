#pragma once

#include "Singleton.hpp"
#include "Singleton_Visualizer.hpp"

class MasterDoorTransition : public single::OverlayState {

    protected:

    single::Sprite wooden_door;

    single::Rect left_door_location;
    single::Rect right_door_location;
    float door_width;
    float door_height;

    double title_display_timer;
    bool doors_closed;
    bool doors_opening;

    bool change_state;
    bool transition_finished;


    public:
    void OnEntry(const single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
    void OnExit() override;
};
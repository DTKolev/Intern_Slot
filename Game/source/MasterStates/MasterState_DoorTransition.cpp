#include "../../headers/MasterStates/MasterState_DoorTransition.hpp"

void MasterDoorTransition::OnEntry(single::Engine& eng) {

    wooden_door = eng.LoadSprite("../src/wooden_tiles.png");

    door_width = 500.0f;
    door_height = 800.0f;

    left_door_location = {-door_width, 0.0f, door_width, door_height};
    right_door_location = {1000.0f, 0.0f, door_width, door_height};

    title_display_timer = 1.0;
    doors_closed = false;
    doors_opening = false;

    change_state = false;
    transition_finished = false;
}

void MasterDoorTransition::HandleInput(single::Engine& eng, SDL_Event& input_event) {}

void MasterDoorTransition::Update(single::Engine& eng, double delta_t) {

    constexpr double door_movement_speed = 700.0;

    change_state = false;

    if (!doors_closed && !doors_opening) {

        left_door_location.x += door_movement_speed * delta_t;
        right_door_location.x -= door_movement_speed * delta_t;

        if (left_door_location.x >= 0.0) {

            left_door_location.x = 0.0;
            right_door_location.x = door_width;
            doors_closed = true;

            change_state = true;
        }
    }

    if (doors_closed) {
        title_display_timer -= delta_t;

        if (title_display_timer <= 0.0) {
            doors_closed = false;
            doors_opening = true;
        }
    }

    if (doors_opening) {

        left_door_location.x -= door_movement_speed * delta_t;
        right_door_location.x += door_movement_speed * delta_t;

        if (left_door_location.x <= -door_width) transition_finished = true;
    }
}

void MasterDoorTransition::Render(single::Engine& eng) {

    eng.RenderSprite(wooden_door, &left_door_location);
    eng.RenderSprite(wooden_door,&right_door_location);
}

void MasterDoorTransition::OnExit() {}
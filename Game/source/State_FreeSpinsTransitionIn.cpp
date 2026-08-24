#include "../headers/Mode_FreeSpins.hpp"
#include <string>

void FreeSpinsTransitionIn::OnEntry(single::Engine& eng) {

    wooden_door = eng.LoadSprite("../src/wooden_tiles.png");
    title = eng.CreateText("FREE SPINS MODE", 64.0f, eng.RandomColor());

    door_width = 500.0f; // half window width
    door_height = 800.0f; // widow height

    left_door_location = {-door_width, 0.0f, door_width, door_height};
    right_door_location = {2.0f * door_width, 0.0f, door_width, door_height};

    title_display_timer = 1.5;
    doors_closed = false;
    doors_opening = false;
}
 
void FreeSpinsTransitionIn::HandleInput(single::Engine& eng, SDL_Event& input_event) {}

void FreeSpinsTransitionIn::Update(single::Engine& eng, double delta_t) {

    constexpr double door_movement_speed = 700.0;

    if (!doors_opening && !doors_closed) {

        left_door_location.x += door_movement_speed * delta_t;
        right_door_location.x -= door_movement_speed * delta_t;

        if(left_door_location.x >= 0.0f) {

            left_door_location.x = 0.0f;
            right_door_location.x = door_width;
            doors_closed = true;

            eng.StateChange<FreeSpinsEntry>();
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

        if (left_door_location.x <= -door_width) eng.StopOverlay();
    }
}

void FreeSpinsTransitionIn::Render(single::Engine& eng) {

    eng.RenderSprite(wooden_door, &left_door_location);
    eng.RenderSprite(wooden_door, &right_door_location);

    if (doors_closed) {

        float x_pos = 500.0f - (title.GetWidth() / 2.0f);

        eng.RenderText(title, x_pos, 400.0f);
    }
}

void FreeSpinsTransitionIn::OnExit() {}
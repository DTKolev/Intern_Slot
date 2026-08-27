#include "../headers/GameModes/Mode_FreeSpins.hpp"
#include "../headers/GameModes/Mode_Regular.hpp"
#include "../headers/GameObjects/CommonManager.hpp"
#include <string>

void FreeSpinsTransitionOut::OnEntry(single::Engine& eng) {
 
    CommonManager& common_manager = CommonManager::GetInstance();
    
    wooden_door = eng.LoadSprite("../src/wooden_tiles.png");
    title = eng.CreateText("TOTAL CREDITS WON", 64.0f, eng.RandomColor());
    winnings = eng.CreateText(std::to_string(common_manager.free_spins_winnings), 64.0f, eng.RandomColor());

    door_width = 500.0f;
    door_height = 800.0f;

    left_door_location = {-door_width, 0.0f, door_width, door_height};
    right_door_location = {1000.0f, 0.0f, door_width, door_height};

    title_display_timer = 2.0f;
    doors_closed = false;
    doors_opening = false;
}

void FreeSpinsTransitionOut::HandleInput(single::Engine& eng, SDL_Event& input_event) {}

void FreeSpinsTransitionOut::Update(single::Engine& eng, double delta_t) {

    CommonManager& common_manager = CommonManager::GetInstance();

    constexpr double door_movement_speed = 700.0;

    if (!doors_opening && !doors_closed) {

        left_door_location.x += door_movement_speed * delta_t;
        right_door_location.x -= door_movement_speed * delta_t;

        if(left_door_location.x >= 0.0f) {

            left_door_location.x = 0.0f;
            right_door_location.x = door_width;
            doors_closed = true;

            common_manager.credits += common_manager.free_spins_winnings;            
            eng.StateChange<Betting>();
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

void FreeSpinsTransitionOut::Render(single::Engine& eng) {

    eng.RenderSprite(wooden_door, &left_door_location);
    eng.RenderSprite(wooden_door, &right_door_location);

    if (doors_closed) {

        float title_x = 500.0f - (title.GetWidth() / 2.0f);
        float winnings_x = 500.0f - (winnings.GetWidth() / 2.0f);

        eng.RenderText(title, title_x, 400.0f);
        eng.RenderText(winnings, winnings_x, 410.0f + title.GetHeight());
    }
}

void FreeSpinsTransitionOut::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.free_spins_winnings = 0;
}
#pragma once

#include "../MasterStates/MasterState_Reeling.hpp"
#include "../MasterStates/MasterState_Results.hpp"
#include <vector>

class ClosingDoorsTransition : public single::GameState {

    protected:
    single::Sprite wooden_door;
    single::Text title;

    single::Rect left_door_location;
    single::Rect right_door_location;
    float door_width;
    float door_height;

    double title_display_timer;
    bool doors_closed;
    bool doors_opening;

    bool change_state;


    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class FreeSpinsTransitionIn : public single::GameState {

    private:
    single::Sprite wooden_door;
    single::Text title;

    single::Rect left_door_location;
    single::Rect right_door_location;

    float door_width;
    float door_height;

    double title_display_timer;
    bool doors_closed;
    bool doors_opening;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class FreeSpinsEntry : public single::GameState {

    private:
    single::Text prompt;
    single::Text bet;
    single::Text free_spins;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class FreeSpinsReeling : public MasterReeling{

    private:
    single::Text winnings;
    single::Text free_spins;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
};

class FreeSpinsResults : public MasterResults {

    private:
    single::Text winnings;
    single::Text free_spins;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class FreeSpinsTransitionOut : public single::GameState {

    private:
    single::Sprite wooden_door;
    single::Text title;
    single::Text winnings;

    single::Rect left_door_location;
    single::Rect right_door_location;

    float door_width;
    float door_height;

    double title_display_timer;
    bool doors_closed;
    bool doors_opening;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};
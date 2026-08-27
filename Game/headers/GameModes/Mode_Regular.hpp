#pragma once

#include "MasterStates/MasterState_Reeling.hpp"
#include "MasterStates/MasterState_Results.hpp"

class Betting : public single::GameState {

    private:
    single::Text credits;
    single::Text bet;
    single::Text title;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class MainMenu : public single::GameState {

    private:
    single::Text title;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class Reeling : public MasterReeling {

    private:
    single::Text credits;
    single::Text title;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
};

class Results : public MasterResults {

    private:
    single::Text credits;
    single::Text title;
    single::Text free_spins;
    single::Text extra_reel;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Render(single::Engine& eng) override;
};
#pragma once

#include "../MasterStates/MasterState_Reeling.hpp"
#include "../MasterStates/MasterState_Results.hpp"
#include "../MasterStates/MasterState_DoorTransition.hpp"
#include "Singleton_Text.hpp"



class FreeSpinsTransitionIn : public MasterDoorTransition {

    private:
    single::Text title;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
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

class FreeSpinsTransitionOut : public MasterDoorTransition {

    private:
    single::Text title;
    single::Text winnings;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};
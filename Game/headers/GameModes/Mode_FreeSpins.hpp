#pragma once

#include "MasterStates/MasterState_Reeling.hpp"
#include "MasterStates/MasterState_Results.hpp"
#include "MasterStates/MasterState_DoorTransition.hpp"
#include "Singleton_Visualizer.hpp"

class FreeSpinsTransitionIn : public MasterDoorTransition {

    private:
    single::Text title;
    bool test;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
};

class FreeSpinsEntry : public single::GameState {

    private:
    single::Visualizer& vis = single::Visualizer::GetInstance();

    single::Text prompt;
    single::Text bet;
    single::Text free_spins;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
    void OnExit() override;
};

class FreeSpinsReeling : public MasterReeling{

    private:
    single::Text winnings;
    single::Text free_spins;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
};

class FreeSpinsResults : public MasterResults {

    private:
    single::Text winnings;
    single::Text free_spins;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Render() override;
    void OnExit() override;
};

class FreeSpinsTransitionOut : public MasterDoorTransition {

    private:
    single::Text title;
    single::Text winnings;

    public:
    void OnEntry(single::Engine& eng) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
    void OnExit() override;
};
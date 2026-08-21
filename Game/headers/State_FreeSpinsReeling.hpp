#pragma once

#include "../../Singleton/Singleton.hpp"

class FreeSpinsReeling : public single::GameState {

    private:
    single::Text winnings;
    single::Text free_spins;


    double timer;
    bool reeling;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};
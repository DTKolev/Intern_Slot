#pragma once

#include "../../Singleton/Singleton.hpp"
#include "Grid.hpp"

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
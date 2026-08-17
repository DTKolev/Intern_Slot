#pragma once

#include "../../Singleton/Singleton.hpp"

class Results : public single::GameState {

    private:
    single::Text credits;
    single::Text bet;
    single::Text win;
    single::Text title;

    int win_amount;

    int display_win;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};
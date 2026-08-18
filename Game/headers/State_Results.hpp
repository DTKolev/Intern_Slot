#pragma once

#include "../../Singleton/Singleton.hpp"
#include <vector>

class Results : public single::GameState {

    using Line = std::vector<int>;

    private:
    single::Text credits;
    single::Text bet;
    single::Text win;
    single::Text title;

    int win_line_amount;
    int current_win_line;
    double timer;
    single::Color curren_win_line_color;

    int win_amount;

    int display_win;

    void DrawLine(single::Engine& eng, const Line& ln, single::Color color) const;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};
#pragma once

#include "Singleton.hpp"

class TestState : public single::GameState {

    private:
    single::Text log;
    double timer;

    public:
    void OnEntry(single::Engine& eng) override {

        log = eng.CreateText("2ND OVERLAY STATE!!!", 23.0f, (single::Color){255, 0, 0, 255});
        timer = 1.5;
    }
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override {}
    void Update(single::Engine& eng, double delta_t) override {

        timer -= delta_t;
        if (timer <= 0.0) eng.RemoveOverlayState();
    }
    void Render(single::Engine& eng) override {

        eng.RenderText(log, 10.0f, 10.0f);
    }
    void OnExit() override {};
};
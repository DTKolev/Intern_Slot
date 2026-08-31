#pragma once

#include "Singleton.hpp"
#include "Singleton_GameState.hpp"

class AnotherTestState;

class TestState : public single::OverlayState {

    private:
    single::Text log;
    double timer;
    bool other_test;

    public:
    void OnEntry(single::Engine& eng) override {

        log = eng.CreateText("2ND OVERLAY STATE!!!", 23.0f, (single::Color){255, 0, 0, 255});
        timer = 1.5;

    }
    void Update(single::Engine& eng, double delta_t) override {

        timer -= delta_t;
        if (timer <= 1.0 && !other_test) {
            eng.AddOverlayState<AnotherTestState>();
            other_test = true;
        }
        if (timer <= 0.0) eng.RemoveOverlayState();
    }
    void Render(single::Engine& eng) override {

        eng.RenderText(log, 10.0f, 10.0f);
    }
    void OnExit() override {};
};

class AnotherTestState : public single::OverlayState {

    private:
    single::Text log;
    double timer;

    public:
    void OnEntry(single::Engine& eng) override {

        log = eng.CreateText("3RD OVERLAY STATE!!!", 23.0f, (single::Color){0, 0, 255, 255});
        timer = 1.5;
        covers_entire_screen = true;
    }
    void Update(single::Engine& eng, double delta_t) override {

        timer -= delta_t;
        if (timer <= 0.0) {
            eng.RemoveOverlayState();
            covers_entire_screen = false;
        }
    }
    void Render(single::Engine& eng) override {

        eng.RenderText(log, 700.0f, 10.0f);
    }
    void OnExit() override {};
};
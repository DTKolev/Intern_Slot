#pragma once

#include "SDL3/SDL_events.h"
#include "Singleton_Visualizer.hpp"

namespace single {

    class Engine;

    class GameState {

        protected:
        Visualizer& vis = Visualizer::GetInstance();

        public:
        GameState() = default;
        virtual ~GameState() = default;

        virtual void OnEntry(const Engine& eng) = 0;
        virtual void HandleInput(Engine& eng, SDL_Event& input_event) = 0;
        virtual void Update(Engine& eng, double delta_t) = 0;
        virtual void Render() = 0;
        virtual void OnExit() = 0;
    };

    class OverlayState {

        protected:
        Visualizer& vis = Visualizer::GetInstance();

        bool covers_entire_screen;

        friend class Engine;

        public:
        OverlayState() : covers_entire_screen{false} {}
        ~OverlayState() = default;

        virtual void OnEntry(const Engine& eng) = 0;
        virtual void Update(Engine& eng, double delta_t) = 0;
        virtual void Render() = 0;
        virtual void OnExit() = 0;
    };
}
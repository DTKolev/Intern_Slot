#pragma once

#include "SDL3/SDL_events.h"
#include <memory>
#include <vector>

namespace single {

    class Engine;

    class GameState {

        public:
        GameState() = default;
        virtual ~GameState() = default;

        virtual void OnEntry(Engine& eng) = 0;
        virtual void HandleInput(Engine& eng, SDL_Event& input_event) = 0;
        virtual void Update(Engine& eng, double delta_t) = 0;
        virtual void Render(Engine& eng) = 0;
        virtual void OnExit() = 0;
    };

    class OverlayStack {

        protected:
        std::vector<std::unique_ptr<GameState>> container;

        public:
        OverlayStack() = default;
        ~OverlayStack() = default;

        void Push(std::unique_ptr<GameState> overlay_state);
        void Pop();
        void Flush();

        GameState* Top() const ;
        GameState* At(int idx) const;

        bool Empty() const;
        int Size() const;
    };
}
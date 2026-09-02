#pragma once

#include <string>
#include <list>
#include <memory>
#include <vector>

#include "Singleton_Common.hpp"
#include "Singleton_Sprite.hpp"
#include "Singleton_Text.hpp"
#include "Singleton_GameState.hpp"
#include "Singleton_TimeManager.hpp"
#include "Singleton_Visualizer.hpp"

namespace single {

    enum class EngineState {
        on,
        off
    };

    class Engine {

        using OverlayStatesList = std::list<std::unique_ptr<OverlayState>>;

        private:
        SDLContextPtr context;

        WindowPtr window;

        EngineState current_state;
        TimeManager time_manager;

        std::unique_ptr<GameState> current_game_state;
        OverlayStatesList overlay_states;

        auto RenderOverlayStates(OverlayStatesList::const_iterator start) const -> void;
        auto FindHighestFullCover() const -> OverlayStatesList::const_iterator;

        friend class Visualizer;

        public:
        Engine(std::string window_title, int window_w, int window_h);

        auto RandomNumber(int high, int low = 0) const -> int;
        void Delay(int ms) const;

        void Run();
        void Quit();



        template<typename T>
        void Init() {

            static_assert(std::is_base_of<GameState, T>::value);

            std::unique_ptr<GameState> start_state = std::make_unique<T>();
            
           current_game_state = std::move(start_state);
           if (current_game_state != nullptr) current_game_state->OnEntry(*this); 
        }

        template<typename T>
        void StateChange() {

            static_assert(std::is_base_of<GameState, T>::value);
            std::unique_ptr<GameState> new_state = std::make_unique<T>();

            if (current_game_state != nullptr) current_game_state->OnExit();
            current_game_state = std::move(new_state);
            if (current_game_state != nullptr) current_game_state->OnEntry(*this);
        }

        template<typename T>
        void AddOverlayState() {

            static_assert(std::is_base_of<OverlayState, T>::value);
            std::unique_ptr<OverlayState> new_overlay_state = std::make_unique<T>();

            overlay_states.push_back(std::move(new_overlay_state));
            if (overlay_states.back() != nullptr) overlay_states.back()->OnEntry(*this);
        }

        void RemoveOverlayState() {

            if (overlay_states.back() != nullptr) overlay_states.back()->OnExit();
            overlay_states.pop_back();
        }

        void StopOverlay() {

            if (!overlay_states.empty()) {

                for (auto& state : overlay_states) {
                    if (state != nullptr) state->OnExit();
                }

                overlay_states.clear();
            }
        }
    };
}
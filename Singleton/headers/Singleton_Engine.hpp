#pragma once

#include <string>
#include <list>
#include <memory>

#include "Singleton_Common.hpp"
#include "Singleton_Sprite.hpp"
#include "Singleton_Text.hpp"
#include "Singleton_GameState.hpp"
#include "Singleton_TimeManager.hpp"
#include "Singleton_Visualizer.hpp"

namespace single {

// Enumerator class for the two possible engine states
enum class EngineState {
    on,
    off
};

// INTERNAL STRUCTURE OF THE ENGINE CLASS
//
// The Engine class is the core of the Singleton engine.
// It's responsible for initializing the SDL3 sub-system,
// creating a window for the program to be displayed in,
// for maintaining the continous game-loop and for managing
// the active game states
//
// It ensures that the SDL3 sub-system will be initialized
// before any other SDL resources and destroyed after they
// have been deallocated. The program will crash otherwise
//
// The core of the Engine class is the Run method. It contains the
// continous program loop and is the part responsible for
// updating and rendering the active game states
//
// The transitions between game states are handled by templated
// helper methods that ensure that the OnEntry and OnExit
// methods of the respective game states are called at the right time.
// These templated methods also maintain the list of overlay states
// and add/remove states from it

class Engine {

    using OverlayStatesList = std::list<std::unique_ptr<OverlayState>>;

    private:
    SDLContextPtr context;

    WindowPtr window;

    EngineState current_state;
    TimeManager time_manager;

    std::unique_ptr<GameState> current_game_state;
    OverlayStatesList overlay_states;

    const std::string path_to_binary;

    auto RenderOverlayStates(OverlayStatesList::const_iterator start) const -> void;
    auto FindHighestFullCover() const -> OverlayStatesList::const_iterator;

    friend class Visualizer;

    public:
    Engine(std::string window_title, int window_w, int window_h);

    auto RandomNumber(int high, int low = 0) const -> int;
    void Delay(int ms) const;
    auto GetBinaryPath() const -> const std::string&;

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
} // end of namespace single

#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Singleton_Common.hpp"
#include "Singleton_GameState.hpp"
#include "Singleton_Visualizer.hpp"
#include <memory>
#include <variant>

using namespace single;

Engine::Engine(std::string window_title, int window_w, int window_h) :
    context{std::make_unique<SDLContext>()},
    window{nullptr},
    current_state{EngineState::on},
    time_manager{},
    current_game_state{nullptr},
    overlay_states{}
{

    Visualizer& vis = Visualizer::GetInstance();

    window = std::make_unique<SDLObject<SDL_Window>>("Slot Game", window_w, window_h);

    vis.Init(*this);
}



int Engine::RandomNumber(int high, int low) const {

    int range = high - low + 1;

    return SDL_rand(range) + low;
}

void Engine::Delay(int ms) const {

    SDL_Delay(ms);
}



void Engine::RenderOverlayStates(OverlayStatesList::const_iterator start) const {

    for (auto it = start; it != overlay_states.end(); it++) {
        it->get()->Render();
    }
}

Engine::OverlayStatesList::const_iterator Engine::FindHighestFullCover() const {

    auto highest_cover = overlay_states.end();

    for (auto it = overlay_states.begin(); it != overlay_states.end(); it++) {
        if (it->get()->covers_entire_screen) highest_cover = it;
    }

    return highest_cover;
}


void Engine::Run() {

    Visualizer& vis = Visualizer::GetInstance();

    while (current_state == EngineState::on) {

        SDL_Event input_event;

        while (SDL_PollEvent(&input_event)) {

            if (input_event.type == SDL_EVENT_QUIT) Quit();

            if (overlay_states.empty()) current_game_state->HandleInput(*this, input_event);
        }

        if(current_state == EngineState::off) break;

        SDL_SetRenderDrawColor(vis.renderer->Get(), 0, 0, 0, 255);
        SDL_RenderClear(vis.renderer->Get());

        if (!overlay_states.empty()) overlay_states.back()->Update(*this, time_manager.DeltaTime());
        else current_game_state->Update(*this, time_manager.DeltaTime());

        auto highest_full_cover = FindHighestFullCover();

        if (highest_full_cover != overlay_states.end()) {
            RenderOverlayStates(highest_full_cover);
        }
        else {

            current_game_state->Render();
            RenderOverlayStates(overlay_states.begin());
        }

        SDL_RenderPresent(vis.renderer->Get());

        time_manager.CalcuateDeltaTime();
    }
}



void Engine::Quit() {

    current_state = EngineState::off;
    Visualizer::GetInstance().Shutdown();    
}
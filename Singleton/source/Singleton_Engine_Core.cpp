#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Singleton_Common.hpp"
#include "Singleton_GameState.hpp"
#include <memory>
#include <utility>

using namespace single;

Engine::Engine(std::string window_title, int window_w, int window_h) :
    context{std::make_unique<SDLContext>()},
    window{nullptr}, renderer{nullptr}, font{nullptr},
    current_state{EngineState::on},
    time_manager{},
    current_game_state{nullptr},
    overlay_states{}
{

    window = std::make_unique<SDLObject<SDL_Window>>("Slot Game", window_w, window_h);
    renderer = std::make_unique<SDLObject<SDL_Renderer>>(window->Get());
    font = std::make_unique<SDLObject<TTF_Font>>("../src/JetBrainsMono-Bold.ttf", 24.0);

    TexturePtr lin_v = std::make_unique<SDLObject<SDL_Texture>>(
        renderer->Get(), 100.0f, 100.0f, 0.6f, GradientType::linear_vertical
    );
    TexturePtr lin_h = std::make_unique<SDLObject<SDL_Texture>>(
        renderer->Get(), 100.0f, 100.0f, 0.6f, GradientType::linear_horizontal
    );
    TexturePtr cen_v = std::make_unique<SDLObject<SDL_Texture>>(
        renderer->Get(), 100.0f, 100.0f, 0.6f, GradientType::centered_vertical
    );
     TexturePtr cen_h = std::make_unique<SDLObject<SDL_Texture>>(
        renderer->Get(), 100.0f, 100.0f, 0.6f, GradientType::centered_horizontal
    );

    gradient_textures.push_back(std::move(lin_v));
    gradient_textures.push_back(std::move(lin_h));
    gradient_textures.push_back(std::move(cen_v));
    gradient_textures.push_back(std::move(cen_h));
}



int Engine::RandomNumber(int high, int low) const {

    int range = high - low + 1;

    return SDL_rand(range) + low;
}

void Engine::Delay(int ms) const {

    SDL_Delay(ms);
}



void Engine::RenderOverlayStates(std::list<std::unique_ptr<OverlayState>>::iterator start) {

    for (auto it = start; it != overlay_states.end(); it++) {
        it->get()->Render(*this);
    }
}

std::list<std::unique_ptr<OverlayState>>::iterator Engine::FindHighestFullCover() {

    auto highest_cover = overlay_states.end();

    for (auto it = overlay_states.begin(); it != overlay_states.end(); it++) {
        if (it->get()->covers_entire_screen) highest_cover = it;
    }

    return highest_cover;
}


void Engine::Run() {

    while (current_state == EngineState::on) {

        SDL_Event input_event;

        while (SDL_PollEvent(&input_event)) {

            if (input_event.type == SDL_EVENT_QUIT) Quit();

            if (overlay_states.empty()) current_game_state->HandleInput(*this, input_event);
        }

        SDL_SetRenderDrawColor(renderer->Get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer->Get());

        if (!overlay_states.empty()) overlay_states.back()->Update(*this, time_manager.DeltaTime());
        else current_game_state->Update(*this, time_manager.DeltaTime());

        auto highest_full_cover = FindHighestFullCover();

        if (highest_full_cover != overlay_states.end()) {
            RenderOverlayStates(highest_full_cover);
        }
        else {

            current_game_state->Render(*this);
            RenderOverlayStates(overlay_states.begin());
        }

        SDL_RenderPresent(renderer->Get());

        time_manager.CalcuateDeltaTime();
    }
}



void Engine::Quit() {

    current_state = EngineState::off;
}
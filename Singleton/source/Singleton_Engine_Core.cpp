#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

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

}



int Engine::RandomNumber(int high, int low) const {

    int range = high - low + 1;

    return SDL_rand(range) + low;
}

void Engine::Delay(int ms) const {

    SDL_Delay(ms);
}



void Engine::Run() {

    while (current_state == EngineState::on) {

        SDL_Event input_event;

        while (SDL_PollEvent(&input_event)) {

            if (input_event.type == SDL_EVENT_QUIT) Quit();

            if (!overlay_states.Empty()) overlay_states.Top()->HandleInput(*this, input_event);
            else current_game_state->HandleInput(*this, input_event);
        }

        SDL_SetRenderDrawColor(renderer->Get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer->Get());

        if (!overlay_states.Empty()) overlay_states.Top()->Update(*this, time_manager.DeltaTime());
        else current_game_state->Update(*this, time_manager.DeltaTime());

        current_game_state->Render(*this);
        if (!overlay_states.Empty()) {
            for (int i = 0; i < overlay_states.Size(); i++) overlay_states.At(i)->Render(*this);
        }

        SDL_RenderPresent(renderer->Get());

        time_manager.CalcuateDeltaTime();
    }
}



void Engine::Quit() {

    current_state = EngineState::off;
}
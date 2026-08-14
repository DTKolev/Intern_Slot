#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

using namespace single;

Text Engine::CreateText(const std::string txt, float font_sz) const {

    Text new_text{txt, font_sz};

    TTF_SetFontSize(font->Get(), font_sz);

    SurfacePtr temp_surface = std::make_unique<SDLObject<SDL_Surface>>(font->Get(), txt);
    
    try {
        new_text.text_texture = std::make_unique<SDLObject<SDL_Texture>>(renderer->Get(), temp_surface->Get());
    }
    catch (FailedTextureCreate& err) {
        new_text.text_texture.reset(nullptr);

        SDL_Log("%s", err.GetMessage().c_str());
    }

    new_text.width = temp_surface->Get()->w;
    new_text.height = temp_surface->Get()->h;

    return new_text;
}



void Engine::RenderText(const Text& text, float x, float y) const {

    SDL_FRect dest {
        .x = x,
        .y = y,
        .w = text.width,
        .h = text.height
    };

    SDL_RenderTexture(renderer->Get(), text.text_texture->Get(), nullptr, &dest);
}
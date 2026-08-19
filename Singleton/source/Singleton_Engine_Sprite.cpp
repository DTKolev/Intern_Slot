#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"

using namespace single;


// *************************** LOCAL HELPER FUNCTIONS ***************************
SDL_FRect EngineRectToFRect(const Rect& rect) {

    SDL_FRect output {
        .x = (float)rect.x,
        .y = (float)rect.y,
        .w = (float)rect.w,
        .h = (float)rect.h
    };

    return output;
}
// ******************************************************************************


Sprite Engine::LoadSprite(const std::string source_file_path) const {

    Sprite new_sprite;

    try {
        new_sprite.surface = std::make_unique<SDLObject<SDL_Surface>>(source_file_path);
        new_sprite.texture = std::make_unique<SDLObject<SDL_Texture>>(renderer->Get(), new_sprite.surface->Get());
    }
    catch (FailedSurfaceCreate& err) {
        new_sprite.surface.reset(nullptr);
        new_sprite.texture.reset(nullptr);

        SDL_Log("%s", err.GetMessage().c_str());
    }
    catch (FailedTextureCreate& err) {
        new_sprite.texture.reset(nullptr);

        SDL_Log("%s", err.GetMessage().c_str());
    }

    return new_sprite;
}

void Engine::RenderSprite(Sprite& sprite, const Rect* dest_rect) const {

    if (sprite.texture == nullptr) {
        
        try {
            sprite.texture = std::make_unique<SDLObject<SDL_Texture>>(renderer->Get(), sprite.surface->Get());
        }
        catch (FailedTextureCreate& err) {
            sprite.texture.reset(nullptr);

            SDL_Log("%s", err.GetMessage().c_str());
            return;
        }
    }

    SDL_FRect dest_sdl = EngineRectToFRect(*dest_rect);

    SDL_RenderTexture(renderer->Get(), sprite.texture->Get(), nullptr, &dest_sdl);
}
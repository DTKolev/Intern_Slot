#include "../headers/Singleton_Sprite.hpp"
#include "SDL3/SDL.h"

using namespace single;

Sprite::Sprite() : texture{nullptr}, surface{nullptr} {}



Sprite::Sprite(const Sprite& copy_src) : texture{nullptr}, surface{nullptr} {

    surface = std::make_unique<SDLObject<SDL_Surface>>(*copy_src.surface.get());
}

Sprite& Sprite::operator=(const Sprite& copy_src) {

    if (texture != nullptr) {
        texture.reset(nullptr);
    }

    surface = std::make_unique<SDLObject<SDL_Surface>>(*copy_src.surface.get());

    return *this;
}



Sprite::Sprite(Sprite&& move_src) : texture{nullptr}, surface{nullptr} {

    texture = std::move(move_src.texture);
    surface = std::move(move_src.surface);
}

Sprite& Sprite::operator=(Sprite&& move_src) {

    texture = std::move(move_src.texture);
    surface = std::move(move_src.surface);
    
    return *this;
}
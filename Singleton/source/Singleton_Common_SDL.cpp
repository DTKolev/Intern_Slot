#include "../headers/Singleton_Common.hpp"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <cstddef>

using namespace single;

SDL_Color EngineColorToSDL(const Color& color) {

    SDL_Color output {
        .r = (Uint8)color.r,
        .g = (Uint8)color.g,
        .b = (Uint8)color.b,
        .a = (Uint8)color.a
    };

    return output;
}



bool Color::operator==(const Color& other) const {

    return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
}


SDLObject<SDL_Window>::SDLObject(std::string window_title, int width, int height) {

    raw_pointer = nullptr;
    raw_pointer = SDL_CreateWindow(window_title.c_str(), width, height, SDL_WINDOW_RESIZABLE);

    if (raw_pointer == nullptr) throw CriticalError{"Failed to create a window"};
}

SDLObject<SDL_Renderer>::SDLObject(SDL_Window* window, const char* name) {

    raw_pointer = nullptr;
    raw_pointer = SDL_CreateRenderer(window, name);

    if (raw_pointer == nullptr) throw CriticalError{"Failed to create a renderer"};
}



SDLObject<SDL_Surface>::SDLObject(std::string source_file_path) {

    raw_pointer = nullptr;

    std::string base_path {SDL_GetBasePath()};
    std::string file_path = base_path + source_file_path;

    raw_pointer = IMG_Load(file_path.c_str());

    if (raw_pointer == nullptr) throw FailedSurfaceCreate{"Failed to load image from: " + source_file_path};
}
SDLObject<SDL_Surface>::SDLObject(TTF_Font* font, std::string text, Color text_color) {

    raw_pointer = nullptr;
    raw_pointer = TTF_RenderText_Blended(font, text.c_str(), 0, EngineColorToSDL(text_color));

    if (raw_pointer == nullptr) throw FailedSurfaceCreate{"Failed to render text: " + text};
}
SDLObject<SDL_Surface>::SDLObject(const SDLObject<SDL_Surface>& copy_src) {

    raw_pointer = nullptr;
    if (copy_src.raw_pointer != nullptr) {
        raw_pointer = SDL_DuplicateSurface(copy_src.raw_pointer);
    }
}
SDLObject<SDL_Surface>& SDLObject<SDL_Surface>::operator=(const SDLObject<SDL_Surface>& copy_src) {

    if (this == &copy_src) return *this;

    if (raw_pointer != nullptr) SDLDeleter{}(raw_pointer);
    raw_pointer = SDL_DuplicateSurface(copy_src.raw_pointer);

    return *this;
}



SDLObject<SDL_Texture>::SDLObject(SDL_Renderer* renderer, SDL_Surface* surface) {

    raw_pointer = nullptr;
    raw_pointer = SDL_CreateTextureFromSurface(renderer, surface);

    if (raw_pointer == nullptr) throw FailedTextureCreate{};
}

SDLObject<SDL_Texture>::SDLObject(SDL_Renderer* renderer, float w, float h) {

    raw_pointer = nullptr;
    raw_pointer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
    if (raw_pointer == nullptr) throw FailedTextureCreate{};
}



SDLObject<TTF_Font>::SDLObject(std::string source_file_path, float size) {

    raw_pointer = nullptr;

    std::string base_path {SDL_GetBasePath()};
    std::string file_path = base_path + source_file_path;

    raw_pointer = TTF_OpenFont(file_path.c_str(), size);

    if (raw_pointer == nullptr) throw CriticalError{"Failed to load font from: " + source_file_path};
}
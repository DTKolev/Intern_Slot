#include "../headers/Singleton_Common.hpp"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

using namespace single;

//**************************
// Color handling untilities
//**************************

auto EngineColorToSDL(const Color& color) -> SDL_Color {

    SDL_Color output {
        .r = (Uint8)color.r,
        .g = (Uint8)color.g,
        .b = (Uint8)color.b,
        .a = (Uint8)color.a
    };

    return output;
}

auto Color::operator==(const Color& other) const -> bool{

    return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
}


//******************
// SDL CONTEXT CLASS
//******************

SDLContext::SDLContext() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Quit();
        throw CriticalError{"Failed SDL3 init"};
    }

    if (!TTF_Init()) {
        TTF_Quit();
        SDL_Quit();

        throw CriticalError{"Failed TTF init"};
    }

    SDL_srand(SDL_GetPerformanceCounter());
}

SDLContext::~SDLContext() {

    TTF_Quit();
    SDL_Quit();
}



//******************************
// GENERIC SDL RESOURCES DELETER
//******************************

void SDLDeleter::operator()(SDL_Window* window) const {
    if (window != nullptr) SDL_DestroyWindow(window);
}
void SDLDeleter::operator()(SDL_Renderer* renderer) const {
    if (renderer != nullptr) SDL_DestroyRenderer(renderer);
}
void SDLDeleter::operator()(SDL_Surface* surface) const {
    if (surface != nullptr) SDL_DestroySurface(surface);
}
void SDLDeleter::operator()(SDL_Texture* texture) const {
    if (texture != nullptr) SDL_DestroyTexture(texture);
}
void SDLDeleter::operator()(TTF_Font* font) const {
    if (font != nullptr) TTF_CloseFont(font);
}



//********************************
// SDL RESOURCES FACTORY FUNCTIONS
//********************************

auto single::MakeSDLWindow(std::string window_title, int width, int height) -> WindowPtr {

    SDL_Window* raw_pointer = nullptr;
    raw_pointer = SDL_CreateWindow(window_title.c_str(), width, height, SDL_WINDOW_RESIZABLE);

    if (raw_pointer == nullptr) throw CriticalError{"Failed to create a window"};

    return WindowPtr{raw_pointer};
}

auto single::MakeSDLRenderer(SDL_Window* window, const char* name) -> RendererPtr {

    SDL_Renderer* raw_pointer = nullptr;
    raw_pointer = SDL_CreateRenderer(window, name);

    if (raw_pointer == nullptr) throw CriticalError{"Failed to create a renderer"};

    return RendererPtr{raw_pointer};
}



auto single::MakeSDLSurface(std::string source_file_path) -> SurfacePtr {

    SDL_Surface* raw_pointer = nullptr;

    std::string base_path {SDL_GetBasePath()};
    std::string file_path = base_path + source_file_path;

    raw_pointer = IMG_Load(file_path.c_str());

    if (raw_pointer == nullptr) throw FailedSurfaceCreate{"Failed to load image from: " + source_file_path};

    return SurfacePtr{raw_pointer};
}

auto single::MakeSDLSurface(TTF_Font* font, std::string text, Color text_color) -> SurfacePtr {

    SDL_Surface* raw_pointer = nullptr;
    raw_pointer = TTF_RenderText_Blended(font, text.c_str(), 0, EngineColorToSDL(text_color));

    if (raw_pointer == nullptr) throw FailedSurfaceCreate{"Failed to render text: " + text};

    return SurfacePtr{raw_pointer};
}

auto single::CopySDLSurface(SDL_Surface* copy_src) -> SurfacePtr {

    SDL_Surface* raw_pointer = nullptr;
    if (copy_src != nullptr) {
        raw_pointer = SDL_DuplicateSurface(copy_src);
    }

    return SurfacePtr{raw_pointer};
}



auto single::MakeSDLTexture(SDL_Renderer* renderer, SDL_Surface* surface) -> TexturePtr {

    SDL_Texture* raw_pointer = nullptr;
    raw_pointer = SDL_CreateTextureFromSurface(renderer, surface);

    if (raw_pointer == nullptr) throw FailedTextureCreate{};

    return TexturePtr{raw_pointer};
}

auto single::MakeSDLTexture(SDL_Renderer* renderer, float w, float h) -> TexturePtr {

    SDL_Texture* raw_pointer = nullptr;
    raw_pointer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );

    if (raw_pointer == nullptr) throw FailedTextureCreate{};

    return TexturePtr{raw_pointer};
}



auto single::MakteTTFFont(std::string source_file_path, float size) -> FontPtr {

    TTF_Font* raw_pointer = nullptr;

    std::string base_path {SDL_GetBasePath()};
    std::string file_path = base_path + source_file_path;

    raw_pointer = TTF_OpenFont(file_path.c_str(), size);

    if (raw_pointer == nullptr) throw CriticalError{"Failed to load font from: " + source_file_path};

    return FontPtr{raw_pointer};
}

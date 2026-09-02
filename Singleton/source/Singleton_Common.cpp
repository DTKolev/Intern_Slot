#include "../headers/Singleton_Common.hpp"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

using namespace single;

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



Error::Error() : message{"Unspecified error"} {}

Error::Error(std::string msg) : message{msg} {}

const std::string& Error::GetMessage() const {return message;}



FailedSurfaceCreate::FailedSurfaceCreate() : Error("Error creating a surface") {}

FailedTextureCreate::FailedTextureCreate() : Error("Error creating a texture") {}
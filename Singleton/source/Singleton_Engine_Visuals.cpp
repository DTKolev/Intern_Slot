#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include <cmath>

using namespace single;

void Engine::RenderLine(float begin_x, float begin_y, float end_x, float end_y, float thickness, Color color) const {

    float dir_x = end_x - begin_x;
    float dir_y = end_y - begin_y;
    float length = std::sqrt(dir_x * dir_x + dir_y * dir_y);

    if (length == 0.0f) return;

    float normalized_x = dir_x / length;
    float normalized_y = dir_y / length;

    float normal_x = -normalized_y;
    float normal_y = normalized_x;

    float offset_x = normal_x * (thickness / 2.0f);
    float offset_y = normal_y * (thickness / 2.0f);

    SDL_Vertex vertecies[4];

    float vertex_r = color.r / 255.0f;
    float vertex_g = color.g / 255.0f;
    float vertex_b = color.b / 255.0f;
    float vertex_a = color.a / 255.0f;

    vertecies[0] = {{begin_x - offset_x, begin_y - offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[1] = {{begin_x + offset_x, begin_y + offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[2] = {{end_x - offset_x, end_y - offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[3] = {{end_x + offset_x, end_y + offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};

    int indecies[] = {0, 1, 2, 1, 2, 3};

    SDL_RenderGeometry(renderer->Get(), nullptr, vertecies, 4, indecies, 6);
}



void Engine::RenderRect(Rect rect, Color color) const {

    SDL_FRect rect_sdl {
        .x = (float)rect.x,
        .y = (float)rect.y,
        .w = (float)rect.w,
        .h = (float)rect.h
    };

    SDL_SetRenderDrawColor(renderer->Get(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer->Get(), &rect_sdl);
}



Color Engine::RandomColor() const {

    double phase = (double)SDL_rand(100);

    int red = (int)SDL_floor(255 *(0.5 + 0.5 * SDL_sin(phase)));
    int green = (int)SDL_floor(255 * (0.5 + 0.5 * SDL_sin(phase + SDL_PI_D * 2.0 / 3.0)));
    int blue = (int)SDL_floor(255 * (0.5 + 0.5 * SDL_sin(phase + SDL_PI_D * 4.0 / 3.0)));

    return (Color){red, green, blue, 255};
}



void Engine::EnableClippedRendering(Rect clip_area) {

    SDL_Rect clip_sdl {
        .x = (int)clip_area.x,
        .y = (int)clip_area.y,
        .w = (int)clip_area.w,
        .h = (int)clip_area.h
    };

    SDL_SetRenderClipRect(renderer->Get(), &clip_sdl);
}

void Engine::DisableClipping() {

    SDL_SetRenderClipRect(renderer->Get(), nullptr);
}
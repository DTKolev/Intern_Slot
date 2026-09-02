#include "Singleton_Visualizer.hpp"
#include "Singleton_Common.hpp"
#include "Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <cstddef>
#include <memory>
#include <type_traits>

using namespace single;


Visualizer& Visualizer::GetInstance() {

    static Visualizer vis;
    return vis;
}

void Visualizer::Init(const Engine& eng) {

    renderer = std::make_unique<SDLObject<SDL_Renderer>>(eng.window->Get());
    font = std::make_unique<SDLObject<TTF_Font>>("../src/JetBrainsMono-Bold.ttf", 24.0);

    gradient_textures.emplace_back(CreateLinearGradient(100.0f, 100.0f));
    gradient_textures.emplace_back(CreateCenteredGradient(100.0f, 100.0f));
}

void Visualizer::Shutdown() {

    renderer.reset(nullptr);
    font.reset(nullptr);

    for (int i = 0; i < gradient_textures.size(); i++) {
        gradient_textures[i].reset(nullptr);
    }
}




TexturePtr Visualizer::CreateLinearGradient(float w, float h) const {

    TexturePtr output = std::make_unique<SDLObject<SDL_Texture>>(renderer->Get(), w, h);

    SDL_SetRenderTarget(renderer->Get(), output->Get());

    SDL_SetRenderDrawColor(renderer->Get(), 0, 0, 0, 0);
    SDL_RenderClear(renderer->Get());

    SDL_FColor opaque_black {0.0f, 0.0f, 0.0f, 1.0f};
    SDL_FColor transparent {0.0f, 0.0f, 0.0f, 0.0f};

    SDL_Vertex vertices[4] = {
        {{0.0f, 0.0f}, opaque_black, {0, 0}},
        {{w, 0.0}, opaque_black, {0, 0}},
        {{0.0f, h}, transparent, {0, 0}},
        {{h, w}, transparent, {0, 0}}
    };

    int indices[6] = {0, 1, 2, 1, 2, 3};

    SDL_RenderGeometry(renderer->Get(), nullptr, vertices, 4, indices, 6);
    SDL_SetRenderTarget(renderer->Get(), nullptr);

    return output;
}

TexturePtr Visualizer::CreateCenteredGradient(float w, float h) const {

    TexturePtr output = std::make_unique<SDLObject<SDL_Texture>>(renderer->Get(), w, h);

    SDL_SetRenderTarget(renderer->Get(), output->Get());

    SDL_SetRenderDrawColor(renderer->Get(), 0, 0, 0, 0);
    SDL_RenderClear(renderer->Get());

    SDL_FColor opaque_black {0.0f, 0.0f, 0.0f, 1.0f};
    SDL_FColor transparent {0.0f, 0.0f, 0.0f, 0.0f};

    SDL_Vertex vertices[6] = {
        {{0.0f, 0.0f}, opaque_black, {0, 0}},
        {{w, 0.0f}, opaque_black, {0, 0}},
        {{0.0f, h / 2.0f}, transparent, {0, 0}},
        {{w, h / 2.0f}, transparent, {0, 0}},
        {{0.0f, h}, opaque_black, {0, 0}},
        {{w, h}, opaque_black, {0, 0}}
    };

    int indices[12] = {0,1,2,1,2,3,2,3,4,3,4,5};

    SDL_RenderGeometry(renderer->Get(), nullptr, vertices, 6, indices, 12);
    SDL_SetRenderTarget(renderer->Get(), nullptr);

    return output;
}

void Visualizer::RenderGradient(const Rect& dest_rect, float brightness, double angle, const GradientType& type) const {

    SDL_FRect sdl_rect {
        .x = dest_rect.x,
        .y = dest_rect.y,
        .w = dest_rect.w,
        .h = dest_rect.h
    };

    int gradient_idx = static_cast<int>(type);

    Uint8 alpha = (Uint8)(brightness * 255.0f);

    SDL_SetTextureAlphaMod(gradient_textures[gradient_idx]->Get(), alpha);
    SDL_SetTextureBlendMode(gradient_textures[gradient_idx]->Get(), SDL_BLENDMODE_MUL);

    SDL_RenderTextureRotated(
        renderer->Get(), gradient_textures[gradient_idx]->Get(),
        nullptr, &sdl_rect, 
        angle, nullptr, 
        SDL_FLIP_NONE
    );
}



Color Visualizer::RandomColor() const {

    double phase = (double)SDL_rand(100);

    int red = (int)SDL_floor(255 *(0.5 + 0.5 * SDL_sin(phase)));
    int green = (int)SDL_floor(255 * (0.5 + 0.5 * SDL_sin(phase + SDL_PI_D * 2.0 / 3.0)));
    int blue = (int)SDL_floor(255 * (0.5 + 0.5 * SDL_sin(phase + SDL_PI_D * 4.0 / 3.0)));

    return (Color){red, green, blue, 255};
}



void Visualizer::EnableClippedRendering(const Rect& clip_area) const {

    SDL_Rect clip_sdl {
        .x = (int)clip_area.x,
        .y = (int)clip_area.y,
        .w = (int)clip_area.w,
        .h = (int)clip_area.h
    };

    SDL_SetRenderClipRect(renderer->Get(), &clip_sdl);
}

void Visualizer::DisableClipping() const {

    SDL_SetRenderClipRect(renderer->Get(), nullptr);
}



void Visualizer::RenderLine(float begin_x, float begin_y, float end_x, float end_y, float thickness, const Color& color) const {

    float dir_x = end_x - begin_x;
    float dir_y = end_y - begin_y;
    float length = SDL_sqrtf(dir_x * dir_x + dir_y * dir_y);

    if (length == 0.0f) return;

    float normalized_x = dir_x / length;
    float normalized_y = dir_y / length;

    float normal_x = -normalized_y;
    float normal_y = normalized_x;

    float offset_x = normal_x * (thickness / 2.0f);
    float offset_y = normal_y * (thickness / 2.0f);

    float cap_offset_x = normalized_x * (thickness / 2.0f);
    float cap_offset_y = normalized_y * (thickness / 2.0f);

    SDL_Vertex vertecies[6];

    float vertex_r = color.r / 255.0f;
    float vertex_g = color.g / 255.0f;
    float vertex_b = color.b / 255.0f;
    float vertex_a = color.a / 255.0f;

    vertecies[0] = {{begin_x - offset_x, begin_y - offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[1] = {{begin_x + offset_x, begin_y + offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[2] = {{end_x - offset_x, end_y - offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[3] = {{end_x + offset_x, end_y + offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[4] = {{begin_x - cap_offset_x, begin_y - cap_offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};
    vertecies[5] = {{end_x + cap_offset_x, end_y - cap_offset_y}, {vertex_r, vertex_g, vertex_b, vertex_a}, {0, 0}};

    int indecies[] = {0, 1, 2, 1, 2, 3, 0, 1, 4, 2, 3, 5};

    SDL_RenderGeometry(renderer->Get(), nullptr, vertecies, 6, indecies, 12);
}



void Visualizer::RenderRect(const Rect& rect, const Color& color) const {

    SDL_FRect rect_sdl {
        .x = (float)rect.x,
        .y = (float)rect.y,
        .w = (float)rect.w,
        .h = (float)rect.h
    };

    SDL_SetRenderDrawColor(renderer->Get(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer->Get(), &rect_sdl);
}



Sprite Visualizer::LoadSprite(const std::string& source_file_path) const {

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

void Visualizer::RenderSprite(Sprite& sprite, const Rect& dest_rect) const {

    if (sprite.texture == nullptr) {
        
        try {
            if (sprite.surface != nullptr) {
                sprite.texture = std::make_unique<SDLObject<SDL_Texture>>(renderer->Get(), sprite.surface->Get());
            }
        }
        catch (FailedTextureCreate& err) {
            sprite.texture.reset(nullptr);

            SDL_Log("%s", err.GetMessage().c_str());
            return;
        }
    }

    SDL_FRect dest_sdl {
        .x = dest_rect.x,
        .y = dest_rect.y,
        .w = dest_rect.w,
        .h = dest_rect.h
    };

    if (sprite.texture != nullptr) {
        SDL_RenderTexture(renderer->Get(), sprite.texture->Get(), nullptr, &dest_sdl);
    }
}



Text Visualizer::CreateText(const std::string& txt, float font_sz, const Color& text_color) const {

    Text new_text{txt, font_sz};

    TTF_SetFontSize(font->Get(), font_sz);

    SurfacePtr temp_surface = std::make_unique<SDLObject<SDL_Surface>>(font->Get(), txt, text_color);
    
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



void Visualizer::RenderText(const Text& text, float x, float y) const {

    SDL_FRect dest {
        .x = x,
        .y = y,
        .w = text.width,
        .h = text.height
    };

    SDL_FRect shadow_dest {
        .x = dest.x + 2.0f,
        .y = dest.y + 2.0f,
        .w = dest.w,
        .h = dest.h
    };

    SDL_SetTextureColorMod(text.text_texture->Get(), 0, 0, 0);
    SDL_RenderTexture(renderer->Get(), text.text_texture->Get(), nullptr, &shadow_dest);

    SDL_SetTextureColorMod(text.text_texture->Get(), 255, 255, 255);
    SDL_RenderTexture(renderer->Get(), text.text_texture->Get(), nullptr, &dest);
}
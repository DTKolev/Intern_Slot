#pragma once

#include <SDL3/SDL_render.h>
#include <vector>
#include <string>

#include "Singleton_Common.hpp"
#include "Singleton_Sprite.hpp"
#include "Singleton_Text.hpp"



namespace single {

    class Engine;

    class Visualizer {

        private:
        RendererPtr renderer;
        FontPtr font;

        std::vector<TexturePtr> gradient_textures;

        auto CreateLinearGradient(float w, float h) const -> TexturePtr;
        auto CreateCenteredGradient(float w, float h) const -> TexturePtr;

        Visualizer() = default;
        
        friend class Engine;

        public:
        ~Visualizer() = default;
        auto Shutdown() -> void;

        static auto GetInstance() -> Visualizer&;
        auto Init(const Engine& eng) -> void;

        auto RandomColor() const -> Color;

        auto LoadSprite(const std::string& source_file_path) const -> Sprite;
        auto RenderSprite(Sprite& sprite, const Rect& dest_rect) const -> void;
        
        auto CreateText(const std::string& txt, float font_sz, const Color& text_color = {255, 255, 255, 255}) const -> Text;
        auto RenderText(const Text& text, float x, float y) const -> void;

        auto RenderLine(float begin_x, float begin_y, float end_x, float end_y, float thickness, const Color& color) const -> void;
        auto RenderRect(const Rect& rect, const Color& color) const -> void;
        auto RenderGradient(const Rect& dest_rect, float brightness, double angle, const GradientType& type) const -> void;

        auto EnableClippedRendering(const Rect& clip_area) const -> void;
        auto DisableClipping() const -> void;
    };
}
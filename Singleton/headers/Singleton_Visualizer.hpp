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
        void Init(const Engine& eng);
        void Shutdown();

        static auto GetInstance() -> Visualizer&;

        auto RandomColor() const -> Color;

        auto LoadSprite(const std::string& source_file_path) const -> Sprite;
        void RenderSprite(Sprite& sprite, const Rect& dest_rect) const;
        
        auto CreateText(const std::string& txt, float font_sz, const Color& text_color = {255, 255, 255, 255}) const -> Text;
        void RenderText(const Text& text, float x, float y) const;

        void RenderLine(float begin_x, float begin_y, float end_x, float end_y, float thickness, const Color& color) const;
        void RenderRect(const Rect& rect, const Color& color) const;
        void RenderGradient(const Rect& dest_rect, float brightness, double angle, const GradientType& type) const;

        void EnableClippedRendering(const Rect& clip_area) const;
        void DisableClipping() const;
    };
}
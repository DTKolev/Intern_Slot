#pragma once

#include <SDL3/SDL_render.h>
#include <vector>
#include <string>

#include "Singleton_Common.hpp"
#include "Singleton_Sprite.hpp"
#include "Singleton_Text.hpp"



namespace single {

// Forward declaration to avoid including Engine.hpp
class Engine;

// INTERNAL STRUCTURE OF THE VISUALIZER CLASS
//
// The Visualizer is a static class that maintains all the
// rendering functionality of the Singleton engine, including
// the SDL3 renderer. It is designed to be accessed through
// instancing
//
// The system-related data that the Visualizer contains are
// the SDL renderer and a TTF font. These variables will cause
// issues if they are accessed while the SDL3 sub-system is not
// active, so the Visualizer provides Init and Shutdown methods,
// so the allocation/deallocation of those resources can be
// controlled precisely
//
// The Visualizer provides a buch of utility functions for
// the rendering of basic visual elements (like lines or rectangles)
// and handles the creation and rendering of more complex visual elements
// (such as Sprites and Text objects)
//
// It relies on the Engine class to provide a window for the renderer
// to be created in
//
// It also maintains a small library of "nicer" visual elements
// that are generated on the spot and not loaded from a file
// (currently, these are gradient textures)
//
// The Visualizer provides the option to enable clipped rendering
// which makes the rendering of subsequent visual elements constrained
// to an arbitrary rectangle on screen (useful for showing some visual
// elements only partially)

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

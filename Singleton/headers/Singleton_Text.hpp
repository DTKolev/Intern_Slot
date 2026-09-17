#pragma once 

#include "Singleton_Common.hpp"
#include <string>

namespace single {

// Forward declaration to avoid including Engine.hpp and Visualizer.hpp
class Engine;
class Visualizer;


// A text object represents a 2D static text
//
// Its main component is the SDL texture that it contains, since
// this is what gets passed to the Visualizer for rendering
//
// Other member variables represent the metadata of the 
// underlying texture (for easy access and comparison)
//
// Copying of text objects is disabled because of the technicalities
// around duplicating an SDL texture (it's stored in GPU VRAM and
// not in RAM)
//
// The text object supports being updated (new text, size or color)
// and it's generally more optimized than crating a new text object,
// since the Update method checks if the new data is the same as the
// old one and bypasses updating entirely if so

class Text {

    private:
    std::string content;
    float font_size;
    Color color;

    TexturePtr text_texture;

    float width;
    float height;

    friend class Engine;
    friend class Visualizer;

    public:
    Text() = default;
    Text(const std::string& txt, float font_sz, const Color& text_color = {255, 255, 255, 255});

    Text(const Text& copy_src) = delete;
    Text& operator=(const Text& copy_src) = delete;

    Text(Text&& move_src);
    Text& operator=(Text&& move_src);

    void Update(const Visualizer& vis, const std::string& new_txt, float new_sz, const Color& new_color = {255, 255, 255, 255});

    // Useful for alingment of multiple text objects
    auto GetWidth() const -> float {return width;}
    auto GetHeight() const -> float {return height;}
};
} // end of namespace single

#pragma once 

#include "Singleton_Common.hpp"
#include <string>

namespace single {

    class Engine;
    class Visualizer;

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

        float GetWidth() const {return width;}
        float GetHeight() const {return height;}
    };
}
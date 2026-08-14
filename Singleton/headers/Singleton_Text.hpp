#pragma once 

#include "Singleton_Common.hpp"
#include <string>

namespace single {

    class Engine;

    class Text {

        private:
        std::string content;
        float font_size;

        TexturePtr text_texture;

        float width;
        float height;

        friend class Engine;

        public:
        Text() = default;
        Text(std::string txt, float font_sz);

        Text(const Text& copy_src) = delete;
        Text& operator=(const Text& copy_src) = delete;

        Text(Text&& move_src);
        Text& operator=(Text&& move_src);

        void Update(Engine& eng, std::string new_txt, float new_sz);

        float GetWidth() const {return width;}
        float GetHeight() const {return height;}
    };
}
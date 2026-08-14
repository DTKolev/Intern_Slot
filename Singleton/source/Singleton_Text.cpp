#include "../headers/Singleton_Text.hpp"
#include "../headers/Singleton_Engine.hpp"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

using namespace single;

Text::Text(std::string txt, float sz) : content{txt}, font_size{sz}, text_texture{nullptr}, width{0.0}, height{0.0} {}

Text::Text(Text&& move_src) : 
    content{std::move(move_src.content)}, font_size{move_src.font_size},
    text_texture{std::move(move_src.text_texture)},
    width{move_src.width}, height{move_src.height}
{}

Text& Text::operator=(Text&& move_src) {

    content = std::move(move_src.content);
    font_size = move_src.font_size;
    text_texture = std::move(move_src.text_texture);
    width = move_src.width;
    height = move_src.height;

    return *this;
}

void Text::Update(Engine& eng, std::string new_txt, float new_sz) {

    if ((content == new_txt) && (font_size == new_sz)) return;

    Text updated_text = eng.CreateText(new_txt, new_sz);

    *this = std::move(updated_text);
}
#include "Singleton_Text.hpp"
#include "Singleton_Visualizer.hpp"

using namespace single;

Text::Text(const std::string& txt, float sz, const Color& text_color) 
    : content{txt}, font_size{sz}, color{text_color}, text_texture{nullptr}, width{0.0}, height{0.0} 
{}

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

void Text::Update(const Visualizer& vis, const std::string& new_txt, float new_sz, const Color& new_color) {

    if ((content == new_txt) && (font_size == new_sz) && (color == new_color)) return;

    Text updated_text = vis.CreateText(new_txt, new_sz, new_color);

    *this = std::move(updated_text);
}
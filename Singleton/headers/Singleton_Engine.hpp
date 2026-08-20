#pragma once

#include <string>
#include <memory>

#include "Singleton_Common.hpp"
#include "Singleton_Sprite.hpp"
#include "Singleton_Text.hpp"
#include "Singleton_GameState.hpp"
#include "Singleton_TimeManager.hpp"

namespace single {

    enum class EngineState {
        on,
        off
    };

    class Engine {

        private:
        SDLContextPtr context;

        WindowPtr window;
        RendererPtr renderer;
        FontPtr font;

        EngineState current_state;
        TimeManager time_manager;

        std::unique_ptr<GameState> current_game_state;

        public:
        Engine(std::string window_title, int window_w, int window_h);

        Sprite LoadSprite(const std::string source_file_path) const;
        void RenderSprite(Sprite& sprite, const Rect* dest_rect) const;
        void RenderLine(float begin_x, float begin_y, float end_x, float end_y, float thickness, Color color) const;
        Color RandomColor() const;

        Text CreateText(const std::string& txt, float font_sz, const Color& text_color = {255, 255, 255, 255}) const;
        void RenderText(const Text& text, float x, float y) const;

        int RandomNumber(int high, int low = 0) const;
        void Delay(int ms) const;

        void Run();
        void Quit();



        template<typename T>
        void Init() {

            static_assert(std::is_base_of<GameState, T>::value);

            std::unique_ptr<GameState> start_state = std::make_unique<T>();
            
           current_game_state = std::move(start_state);
           if (current_game_state != nullptr) current_game_state->OnEntry(*this); 
        }

        template<typename T>
        void StateChange() {

            static_assert(std::is_base_of<GameState, T>::value);
            std::unique_ptr<GameState> new_state = std::make_unique<T>();

            if (current_game_state != nullptr) current_game_state->OnExit();
            current_game_state = std::move(new_state);
            if (current_game_state != nullptr) current_game_state->OnEntry(*this);
        }
    };
}
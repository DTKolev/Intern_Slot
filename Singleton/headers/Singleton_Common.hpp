#pragma once

#include <memory>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct TTF_Font;

namespace single {

    enum class GradientType {
        linear_vertical,
        linear_horizontal,
        centered_vertical,
        centered_horizontal
    };



    struct Color {
        int r, g, b, a;

        bool operator==(const Color& other) const;
    };

    struct Rect {
        float x, y, w, h;
    };

    struct SDLDeleter {

        void operator()(SDL_Window* window) const;
        void operator()(SDL_Renderer* renderer) const;
        void operator()(SDL_Surface* surface) const;
        void operator()(SDL_Texture* texture) const;
        void operator()(TTF_Font* font) const; 
    };

    class SDLContext {

        public:
        SDLContext();
        ~SDLContext();

        SDLContext(const SDLContext& copy_src) = delete;
        SDLContext& operator=(const SDLContext& copy_src) = delete;

        SDLContext(SDLContext&& move_src) = default;
        SDLContext& operator=(SDLContext&& move_src) = default;
    };



    template <typename T>
    class SDLObjectRoot {

        protected:
        T* raw_pointer;
        SDLObjectRoot() = default;

        public:
        virtual ~SDLObjectRoot() {SDLDeleter{}(raw_pointer);}
        [[nodiscard]] T* Get() const {return raw_pointer;}

        // Placeholder shallow copy constructor and operator=
        SDLObjectRoot(const SDLObjectRoot& copy_src) : raw_pointer{copy_src.raw_pointer} {};
        SDLObjectRoot& operator=(const SDLObjectRoot& copy_src) {
            raw_pointer = copy_src.raw_pointer;
            return *this;
        }

        // Define move constructor and operator=
        SDLObjectRoot(SDLObjectRoot&& move_src) : raw_pointer{move_src.raw_pointer} {
            move_src.raw_pointer = nullptr;
        }
        SDLObjectRoot& operator=(SDLObjectRoot&& move_src) {
            if (this != &move_src) {
                SDLDeleter{}(raw_pointer);
                raw_pointer = move_src.raw_pointer;
                move_src.raw_pointer = nullptr;
            }

            return *this;
        }
    };

    template <typename T>
    class SDLObject {
        static_assert(
            std::is_same<T, SDL_Window>::value ||
            std::is_same<T, SDL_Renderer>::value ||
            std::is_same<T, SDL_Surface>::value ||
            std::is_same<T, SDL_Texture>::value ||
            std::is_same<T, TTF_Font>::value,
            "Unsupported type passed to the SDLObject template"
        );
    };

    template<>
    class SDLObject<SDL_Window> : public SDLObjectRoot<SDL_Window> {
        public:
        SDLObject(std::string window_title, int width, int height);

        // Disable copying for SDL_Window
        SDLObject(const SDLObject& copy_src) = delete;
        SDLObject& operator=(const SDLObject& copy_src) = delete;
    };

    template<>
    class SDLObject<SDL_Renderer> : public SDLObjectRoot<SDL_Renderer> {
        public:
        SDLObject(SDL_Window* window, const char* name = nullptr);

        // Disable copying for SDL_Renderer
        SDLObject(const SDLObject& copy_src) = delete;
        SDLObject& operator=(const SDLObject& copy_src) = delete;
    };

    template<>
    class SDLObject<SDL_Surface> : public SDLObjectRoot<SDL_Surface> {
        public:
        SDLObject(std::string source_file_path);
        SDLObject(TTF_Font* font, std::string text, Color text_color = {255, 255, 255, 255});

        // Allow copying for SDL_Surface
        SDLObject(const SDLObject<SDL_Surface>& copy_src);
        SDLObject& operator=(const SDLObject<SDL_Surface>& copy_src);
    };

    template<>
    class SDLObject<SDL_Texture> : public SDLObjectRoot<SDL_Texture> {
        public:
        SDLObject(SDL_Renderer* renderer, SDL_Surface* surface);
        SDLObject(SDL_Renderer* renderer, float w, float h, float min_br, const GradientType& type);

        // Disable copying for SDL_Texture
        SDLObject(const SDLObject& copy_src) = delete;
        SDLObject& operator=(const SDLObject& copy_src) = delete;
    };

    template<>
    class SDLObject<TTF_Font> : public SDLObjectRoot<TTF_Font> {
        public:
        SDLObject(std::string source_file_path, float size);

        // Disable copying for TTF_Font
        SDLObject(const SDLObject& copy_src) = delete;
        SDLObject& operator=(const SDLObject& copy_src) = delete;
    };



    using SDLContextPtr = std::unique_ptr<SDLContext>;
    using WindowPtr = std::unique_ptr<SDLObject<SDL_Window>>;
    using RendererPtr = std::unique_ptr<SDLObject<SDL_Renderer>>;
    using SurfacePtr = std::unique_ptr<SDLObject<SDL_Surface>>;
    using TexturePtr = std::unique_ptr<SDLObject<SDL_Texture>>;
    using FontPtr = std::unique_ptr<SDLObject<TTF_Font>>;



    class Error {

        protected:
        std::string message;

        public:
        Error();
        Error(std::string msg);

        const std::string GetMessage() const;
    };

    class FailedSurfaceCreate : public Error {

        public:
        FailedSurfaceCreate();
        FailedSurfaceCreate(std::string msg) : Error(msg){}
    };

    class FailedTextureCreate : public Error {

        public:
        FailedTextureCreate();
        FailedTextureCreate(std::string msg) : Error(msg) {}
    };

    class CriticalError : public Error {

        public:
        CriticalError(std::string msg) : Error(msg) {}
    };
}
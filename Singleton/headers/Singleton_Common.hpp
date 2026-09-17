#pragma once

#include <memory>
#include <string>

// Forward declarations to avoid including SDL3.h
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct TTF_Font;

namespace single {

// Global structs shared across the engine logic

enum class GradientType {
    linear,
    centered
};

struct Color {
    int r, g, b, a;

    bool operator==(const Color& other) const;
};

struct Rect {
    float x, y, w, h;
};



// CUSTOM LOGIC FOR MANAGING SDL RESOURCES
//
// The SDL3 sub-system is managed automatically by a wrapper class to
// ensure that it's initialized before and destroyed after all SDL resources
// have been allocated and deallocated
//
// Opaque SDL3 data structures that can only be accessed through pointers
// are wrapped inside std::unique_ptr for automated memory management
//
// SDL3 data structures are created through custom factory functions that
// function similar to std::make_unique, but assign the appropreate SDL
// deleters instead of std::default_delete to the std::unique_ptr that
// they return

// Generic deleter for SDL3 resources
struct SDLDeleter {
    void operator()(SDL_Window* window) const;
    void operator()(SDL_Renderer* renderer) const;
    void operator()(SDL_Surface* surface) const;
    void operator()(SDL_Texture* texture) const;
    void operator()(TTF_Font* font) const; 
};

// Wrapper class for the SDL3 sub-system
class SDLContext {

    public:
    SDLContext();
    ~SDLContext();

    SDLContext(const SDLContext& copy_src) = delete;
    SDLContext& operator=(const SDLContext& copy_src) = delete;

    SDLContext(SDLContext&& move_src) = default;
    SDLContext& operator=(SDLContext&& move_src) = default;
};



// Wrappers around SDL raw pointers for automated memory management
using SDLContextPtr = std::unique_ptr<SDLContext>;
using WindowPtr = std::unique_ptr<SDL_Window, SDLDeleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SDLDeleter>;
using SurfacePtr = std::unique_ptr<SDL_Surface, SDLDeleter>;
using TexturePtr = std::unique_ptr<SDL_Texture, SDLDeleter>;
using FontPtr = std::unique_ptr<TTF_Font, SDLDeleter>;



// Factory functions for creating SDL resources

auto MakeSDLWindow(std::string window_title, int width, int height) -> WindowPtr;

auto MakeSDLRenderer(SDL_Window* window, const char* name = nullptr) -> RendererPtr;

auto MakeSDLSurface(std::string source_file_path) -> SurfacePtr;
auto MakeSDLSurface(TTF_Font* font, std::string text, Color text_color = {255, 255, 255, 255}) -> SurfacePtr;
auto CopySDLSurface(SDL_Surface* copy_src) -> SurfacePtr;

auto MakeSDLTexture(SDL_Renderer* renderer, SDL_Surface* surface) -> TexturePtr;
auto MakeSDLTexture(SDL_Renderer* renderer, float w, float h) -> TexturePtr;

auto MakteTTFFont(std::string source_file_path, float size) -> FontPtr;



// Error handling

class Error {

    protected:
    std::string message;

    public:
    Error();
    Error(std::string msg);
    virtual ~Error() = default;

    auto GetMessage() const -> const std::string&;
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
} // end of namspace single

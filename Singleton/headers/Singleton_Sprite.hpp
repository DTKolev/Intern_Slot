#pragma once

#include "Singleton_Common.hpp"

namespace single {

// Forward declaration to avoid including Engine.hpp and Visualizer.hpp
class Engine;
class Visualizer;

// The sprite class represents a 2D static image asset
//
// It can't render itself or load/create an image to populate
// its member variables with data
//
// It's intended to be stored by its owner and be passed to
// the renderer as a "rendering task". It relies on other classes
// (mainly the Visualizer) to be created and for getting data assigned
// to it

class Sprite {

    private:
    TexturePtr texture;
    SurfacePtr surface;

    friend class Engine;
    friend class Visualizer;

    public:
    Sprite();

    Sprite(const Sprite& copy_src);
    Sprite(Sprite&& move_src);
    Sprite& operator=(const Sprite& copy_src);
    Sprite& operator=(Sprite&& move_src);

    auto Empty() const -> bool;
};
} // end of namespace single

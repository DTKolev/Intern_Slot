#pragma once

#include "Singleton_Common.hpp"

namespace single {

    class Engine;
    class Visualizer;

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

        bool Empty() const;
    };
}
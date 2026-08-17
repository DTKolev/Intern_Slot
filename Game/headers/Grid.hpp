#pragma once

#include "../../Singleton/Singleton.hpp"
#include <vector>

enum class CellContent {
    cherry,
    lemon,
    orange,
    bell,
    seven,
    diamond,
    wild,
    empty
};

struct Cell {
    int column;
    SDL_FRect location;
    CellContent content;
};

class Grid {

    private:
    std::vector<Cell> cells;
    std::vector<single::Sprite> sprites;

    public:
    Grid(int rows, int columns, float size);

    void DrawRNG(single::Engine& eng);
    void RenderGrid(single::Engine& eng);
    const std::vector<CellContent> ExportState() const;
};
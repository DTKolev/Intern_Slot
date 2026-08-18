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

struct GridData {
    int rows;
    int columns;
    float cell_size;
};

class Grid {

    private:
    std::vector<Cell> cells;
    std::vector<single::Sprite> sprites;

    GridData data;

    public:
    Grid(int rows, int columns, float cell_size);

    void DrawRNG(single::Engine& eng);
    void RenderGrid(single::Engine& eng);
    const std::vector<CellContent> ExportState() const;

    const GridData& GetGridData() const {return data;}
};
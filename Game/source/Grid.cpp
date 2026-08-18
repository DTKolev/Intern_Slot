#include "../headers/Grid.hpp"

Grid::Grid(int rows, int columns, float cell_size) :
    data{rows, columns, cell_size}
{

    cells.assign(rows * columns, (Cell){0, {0, 0, cell_size, cell_size}, CellContent::empty});

    int current_row = 0;
    int current_column = 0;

    for (int i = 0; i < cells.size(); i++) {

        current_column = i % columns;

        cells[i].column = current_column;
        cells[i].location.x += (float)(current_column) * (cell_size);
        cells[i].location.y += (float)(current_row) * (cell_size);

        if ((i + 1) % columns == 0) current_row++;
    }
}



void Grid::DrawRNG(single::Engine& eng) {

    for (Cell& c : cells) {
        
        int last_idx = static_cast<int>(CellContent::wild);
        int rand_num;
        
        if (c.column == 0) {
            last_idx--;
        }
        rand_num = eng.RandomNumber(last_idx);

        c.content = static_cast<CellContent>(rand_num);
    }
}



void Grid::RenderGrid(single::Engine& eng) {

    if (sprites.empty()) {
        sprites.assign({
            eng.LoadSprite("../src/cherry.png"),
            eng.LoadSprite("../src/lemon.png"),
            eng.LoadSprite("../src/orange.png"),
            eng.LoadSprite("../src/bell.png"),
            eng.LoadSprite("../src/seven.png"),
            eng.LoadSprite("../src/diamond.png"),
            eng.LoadSprite("../src/wild.png"),
            eng.LoadSprite("../src/empty.png")
        });
    }

    for (const Cell& c : cells) {
        
        int sprite_idx = static_cast<int>(c.content);
        eng.RenderSprite(sprites[sprite_idx], &c.location);
    }
}



const std::vector<CellContent> Grid::ExportState() const {

    std::vector<CellContent> grid_state;
    grid_state.reserve(cells.size());

    for(const Cell& c : cells) {
        grid_state.push_back(c.content);
    } 

    return grid_state;
}
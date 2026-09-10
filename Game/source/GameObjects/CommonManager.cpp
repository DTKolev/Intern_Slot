#include "GameObjects/CommonManager.hpp"
#include <fstream>

CommonManager::CommonManager() : 
    game_grid{0.0f, 0.0f, 3, 5, 200.0f},
    analyzer{},
    credits{100}, bet{1},
    free_spins{0}, free_spins_winnings{0}, free_spins_mode{false},
    extra_reel_mode{false},
    reverse_lines{false}
{
    std::ifstream input_str {"../src/save_file.txt"};
    if (input_str){
        input_str >> credits;
        input_str >> bet;
    }

    const GridData& grid_data = game_grid.GetGridData();

    float cell_size = 175.0f;
    float pos_x = 500.0f - ((float)grid_data.columns * cell_size) / 2.0f;
    float pos_y = 300.0f - ((float)grid_data.rows * cell_size) / 2.0f;
    
    game_grid.RelocateGrid(pos_x, pos_y, cell_size);
}

CommonManager::~CommonManager() {

    std::ofstream output_str {"../src/save_file.txt"};
    output_str << credits << ' ' << bet;
}



CommonManager& CommonManager::GetInstance() {

    static CommonManager manager;
    return manager;
}

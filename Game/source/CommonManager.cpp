#include "../headers/CommonManager.hpp"

CommonManager::CommonManager() : 
    game_grid{0.0f, 0.0f, 3, 5, 200.0f},
    analyzer{},
    credits{100}, bet{1},
    free_spins{0}, free_spins_winnings{0}, free_spins_mode{false}
{}



CommonManager& CommonManager::GetInstance() {

    static CommonManager manager;
    return manager;
}
#include "../headers/CommonManager.hpp"

CommonManager::CommonManager() : 
    game_grid{3, 5, 600.0},
    analyzer{},
    credits{100}, bet{1}
{}



CommonManager& CommonManager::GetInstance() {

    static CommonManager manager;
    return manager;
}
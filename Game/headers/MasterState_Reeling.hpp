#pragma once

#include "../../Singleton/Singleton.hpp"
#include "CommonManager.hpp"
#include "InputManager.hpp"

class MasterReeling : public single::GameState {

    protected:
    CommonManager& common_manager = CommonManager::GetInstance();
    InputManager& input_manager = InputManager::GetInstance();

    double timer;
    bool reeling;

    
    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};
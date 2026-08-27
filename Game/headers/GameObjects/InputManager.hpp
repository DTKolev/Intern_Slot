#pragma once

#include "../../../Singleton/Singleton.hpp"
#include <vector>
#include <unordered_map>

enum class Key {
    up, down, left, right,
    plus, minus,
    escape, enter, shift,
    amount
};

struct Button {
    bool has_changed;
    bool is_down;
};

class InputManager {

    private:
    std::vector<Button> buttons;
    std::unordered_map<SDL_Keycode, Key> bindings;

    InputManager();

    void SetButtonState(Key button_id, bool input_down);
    void ResetButtonStates();

    public:
    InputManager(const InputManager& copy_src) = delete;
    InputManager& operator=(const InputManager& copy_src) = delete;

    static InputManager& GetInstance();

    void ProcessInput(SDL_Event& input_event);

    bool IsPressed(Key button_id) const;
    bool IsReleased(Key button_id) const;
    bool IsDown(Key button_id) const;
};
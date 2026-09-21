#pragma once

#include "Singleton.hpp"
#include <vector>
#include <unordered_map>

enum class Key {
    up, down, left, right,
    plus, minus,
    escape, enter, shift,
    d,
    amount
};

struct Button {
    bool has_changed;
    bool is_down;
};

/**
 * @brief Processes input events and translates them
 * into useful information for the program
 *
 * Maintans a vector of buttons that represent keyboard keys
 * that are handled by the program
 * The input manager will ignore input events that are not
 * useful to the program
 */
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

    static auto GetInstance() -> InputManager&;

    void ProcessInput(SDL_Event& input_event);

    /**
     * @return returns true if the button has switched
     * its state from up to down during the current frame
     */
    auto IsPressed(Key button_id) const -> bool;
    /**
     * @return returns true if the button has switched
     * its state from down to up during the current frame
     */
    auto IsReleased(Key button_id) const -> bool;
    /**
     * @return returns true if the button is being
     * held down during the current frame
     */
    auto IsDown(Key button_id) const -> bool;
};

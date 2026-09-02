#include "GameObjects/InputManager.hpp"

InputManager::InputManager() {

    size_t button_amount = static_cast<size_t>(Key::amount);
    buttons.assign(button_amount, (Button){false, false});

    bindings[SDLK_RETURN] = Key::enter;
    bindings[SDLK_RETURN2] = Key::enter;
    bindings[SDLK_ESCAPE] = Key::escape;
    bindings[SDLK_LSHIFT] = Key::shift;
    bindings[SDLK_RSHIFT] = Key::shift;
    bindings[SDLK_UP] = Key::up;
    bindings[SDLK_DOWN] = Key::down;
    bindings[SDLK_LEFT] = Key::left;
    bindings[SDLK_RIGHT] = Key::right;
    bindings[SDLK_PLUS] = Key::plus;
    bindings[SDLK_MINUS] = Key::minus;
}

InputManager& InputManager::GetInstance() {

    static InputManager manager_instance;
    return manager_instance;
}



void InputManager::SetButtonState(Key button_id, bool input_down) {

    size_t id = static_cast<size_t>(button_id);

    buttons[id].has_changed = buttons[id].is_down != input_down;
    buttons[id].is_down = input_down;
}

void InputManager::ResetButtonStates() {

    for (Button& b : buttons) b.has_changed = false;
}

void InputManager::ProcessInput(SDL_Event& input_event) {

    ResetButtonStates();

    switch (input_event.type) {

        case SDL_EVENT_KEY_UP:
        case SDL_EVENT_KEY_DOWN:
        {
            bool down = input_event.key.down;

            auto key_iterator = bindings.find(input_event.key.key);
            if (key_iterator != bindings.end()) {
                SetButtonState(key_iterator->second, down);
            }

            break;
        }
        default: break;
    }
}



auto InputManager::IsPressed(Key button_id) const -> bool {

    size_t id = static_cast<size_t>(button_id);
    return buttons[id].has_changed && buttons[id].is_down;
}

auto InputManager::IsReleased(Key button_id) const -> bool {

    size_t id = static_cast<size_t>(button_id);
    return buttons[id].has_changed && !buttons[id].is_down;
}

auto InputManager::IsDown(Key button_id) const -> bool {

    size_t id = static_cast<size_t>(button_id);
    return buttons[id].is_down;
}
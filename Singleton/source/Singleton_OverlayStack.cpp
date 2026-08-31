#include "Singleton_GameState.hpp"
#include <cstddef>
#include <memory>

using namespace single;

void OverlayStack::Push(std::unique_ptr<GameState> overlay_state) {

    container.push_back(std::move(overlay_state));
}

void OverlayStack::Pop() {

    container.pop_back();
}

void OverlayStack::Flush() {

    container.clear();
}



GameState* OverlayStack::Top() const {

    if (container.back() == nullptr) return nullptr;

    return container.back().get();
}

GameState* OverlayStack::At(int idx) const {

    if (idx < 0 || idx >= container.size() || container.at(idx) == nullptr) return nullptr;

    return container.at(idx).get();
}



bool OverlayStack::Empty() const {

    return container.empty();
}

int OverlayStack::Size() const {

    return container.size();
}
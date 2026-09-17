#pragma once

#include "SDL3/SDL_events.h"
#include "Singleton_Visualizer.hpp"

namespace single {

// Forward declaration to avoid including Engine.hpp
class Engine;


// INTERNAL STRUCTURE OF SUPPORTED GAME STATES
//
// A game state is a stand-alone element responsible for 
// its own allocation/deallocation of resources (if any)
//
// If a game state is currently not active its state is not kept
// in memory by default. A game state should be able to load the
// needed resources from scratch every time upon entry
//
// Its member functions are called by the Engine class at the
// appropriate times and it is responsible for providing 
// the appropriate data to execute them
//
// OnEntry - called once when the program switched INTO the 
// current state; Resource allocation should happen here
//
// HandleInput - called once per Input Event that has been detected
//
// Update - called once per frame
//
// Render - called once per frame
//
// OnExit - called once when the program switches OUT OF the
// current state; Resource deallocation here is optional if the
// clean-up can be handled by the default destructor

class GameState {

protected:
    Visualizer& vis = Visualizer::GetInstance();

public:
    GameState() = default;
    virtual ~GameState() = default;

    virtual void OnEntry(const Engine& eng) = 0;
    virtual void HandleInput(Engine& eng, SDL_Event& input_event) = 0;
    virtual void Update(Engine& eng, double delta_t) = 0;
    virtual void Render() = 0;
    virtual void OnExit() = 0;
};



// The overlay state class is designed to be used in
// a sequence (implemented using std::list) of other
// overlay states
//
// It's intended for being simulated on top of the current
// game state or other overlay states
//
// By default, the overlay state ignores input events, but
// overriding that behavior is supported
//
// Only the highest overlay state receives an update from
// the Engine every frame (the remaining states appear frozen
// while the highest overlay state is active)
//
// The engine renders the underlying game state and all active
// overlay states from bottom-to-top (the highest overlay state
// occludes the rest)
//
// covers_entire_screen - can be toggled on for optimization;
// it disables the rendering off all active states below the
// current state (remaining overlay states on top of the current
// state will still be rendered)
class OverlayState {

protected:
    Visualizer& vis = Visualizer::GetInstance();

    bool covers_entire_screen;

    friend class Engine;

public:
    OverlayState() : covers_entire_screen{false} {}
    virtual ~OverlayState() = default;

    virtual void OnEntry(const Engine& eng) = 0;
    virtual void HandleInput(single::Engine& eng, SDL_Event& input_event) {}
    virtual void Update(Engine& eng, double delta_t) = 0;
    virtual void Render() = 0;
    virtual void OnExit() = 0;
};
} // end of namespace single

#pragma once

#include "SDL3/SDL_events.h"
#include "Singleton_Visualizer.hpp"

namespace single {

// Forward declaration to avoid including Engine.hpp
class Engine;

/**
 * @brief Interface class for implementing game states supported
 * by the engine
 *
 * A game state is a stand-alone element responsible for
 * its own allocation/deallocation of resources
 *
 * If a game state is currently not active, its state is not kept
 * in memory by default. A game state should be able to load the
 * needed resources from scratch every time upon entry
 *
 * The member functions included in the interface class get called
 * by the engine at the appropriate times. The engine is responsible
 * for providing the needed data for their execution
 */
class GameState {

protected:
    Visualizer& vis = Visualizer::GetInstance();

public:
    GameState() = default;
    virtual ~GameState() = default;

    /**
     * @brief Entry point of a supported game state
     *
     * Gets called once when the program transitions INTO the current game state
     * Resource allocation for the current state should happen here
     * Meant for logic that has to be executed upon entry
     *
     * @param eng provides the functionality for loading resources from files
     */
    virtual void OnEntry(const Engine& eng) = 0;
    /**
     * @brief Translate raw input data from the engine
     *
     * Gets called once per Input Event that has been detected
     * All input events get fed into this function; The implementations of this
     * function are responsible for isolating the events that are relevent to
     * their respective game states
     *
     * @param eng provides the option for switching into another game state
     * if certain input is detected
     * @param input_event input event to be handled
     */
    virtual void HandleInput(Engine& eng, SDL_Event& input_event) = 0;
    /**
     * @brief Update the state of the current game state based
     * on delta time
     *
     * Gets called once per frame
     *
     * @param eng provides the option for switching into another game state
     * if a certain event happens
     */
    virtual void Update(Engine& eng, double delta_t) = 0;
    /**
     * @brief Render the contents of the current game state
     *
     * Gets called once per frame
     */
    virtual void Render() = 0;
    /**
     * @brief Exit point of the current game state
     *
     * Gets called once when the program transitions OUT OF the current
     * game state
     * Resource deallocation is optional if the clean-up can be
     * handled by the current game state's destructor
     * Meant for logic that has to be executed upon exiting the game state
     */
    virtual void OnExit() = 0;
};



/**
 * @brief Interface class for implementing supported overlay
 * game states
 *
 * An overlay state is a game state intended to be displayed on top
 * of another active game state
 *
 * It's designed to be used in a sequence (so multiple overlay states
 * can be active at the same time)
 *
 * By default, an overlay state ignores input, but overriding that
 * behavior is supported
 *
 * Only the top-most overlay state receives updates from the engine
 * (both for input and delta time); The underlying states appear
 * frozen while the top state is active
 *
 * Member functions are the same as the ones in the GameState intrerface
 */
class OverlayState {

protected:
    Visualizer& vis = Visualizer::GetInstance();

    /**
     * @brief Flag for rendering optimization
     *
     * If toggled on, the engine will not render any states
     * that are "below" the current state
     */
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

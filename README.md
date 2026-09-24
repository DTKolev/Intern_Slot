# Slot Game Project

### Description
This project was developed as part of my preparation for a position inside the casino games industry. It is a 3x5 slot reeling game that features 3 different game modes, animated game states and transitions, saving/loading functionality and a dedicated debug mode.
The functionality of the game was inspired by real-world lottery games and terminals and simulates a stripped down version of a regular reeling game.

The game runs inside my own custom game engine that is configured as a custom library that I can include into any game project that I make.
The engine handles the SDL3 sub-system and manages SDL resources. It facilitates the low-level implementation of the game loop and the rendering and exposes utility functions to its user. The engine guarantees memory safety and cross-platform usage of the program.

### The Engine
My game engine consists of 3 main parts: the engine, the visualizer and the game state interface.

The engine part handles the SDL3 sub-system, the window creation, the game loop and the game states.
- SDL3 sub-system - SLD3 is a cross-platform C library and all of its functions and processes run inside a sub-system. If this sub-system is not active, the program will crash with a segmentation fault, since no memory can be allocated/deallocated in that case. The engine part ensures that the SDL3 sub-system will be initialized before any of the SDL functions/resources are used and destroyed after all resources have been completely deallocated from memory.
- Windowing - Since the program is intended as a GUI (Graphical User Interface), it has to run inside its own dedicated window. This window will be the parent of all visual elements of the game, and the way that SDL works is that a window has to be created first, so a renderer can attach to it (a renderer can't exist as a stand-alone element). The engine part creates a window immediately after the SDL3 sub-system is initialized.
- Game loop - A game should maintain a continuous loop of processing input, updating the game and rendering to screen. The engine part kick-starts that loop when all of its resources have been allocated successfully and maintains it until the user decides to quit the game. The game loop ensures that the above-mentioned cycle is executed appropriately and that the correct data gets distributed to the currently-active game state.
- Game state interface - The engine provides a blueprint for creating different game states to its user. A game state has the following functions: Entry, Input processing, Update (based on delta time), Render and Exit. The engine is responsible for calling those functions at the appropriate times (that will be explained in more detail in the following section).

The visualizer handles the rendering of the program and provides visual utility functions to its user
- Rendering - The visualizer creates a renderer and manages it for the duration of the program lifetime. An SDL Renderer is an opaque interface for the GPU, so some abstractions have to be built around its low-level implementation, so using it becomes easier.
- Sprites - The visualizer supports loading images from files and displaying them on the screen. This is the main form of visualization since this is a 2D engine. The sprites are objects in memory that store a GPU texture that can be passed to the renderer at any point for visualization (the visualizer handles that process).
- Graphical utilities - The visualizer also provides some utility functions for drawing basic graphical elements such as: a line from point A to B, a rectangle, a gradient and so on. These visual elements are not loaded from files, but are generated procedurally instead.

### The Game State Interface
The implementation of the game states deserves a bit more attention because it relies on inheritance and C++ templates.
The engine provides a parent, purely virtual (interface) game state class. This is the blueprint for creating any game states that the user wishes to make. This class contains the 5 above-mentioned methods (entry, input, update, render and exit) and provides no implementation for them (they are purely virtual)
The engine stores an internal pointer to this interface class and calls the contained object's methods at the appropriate times. Because of the way that inheritance works, any derived game state object can be stored in that internal pointer and the engine will be able to call its methods by referencing the parent object.
The transition between game states is handled by templated functions inside the engine class. The way they work is the following: the user calls the transition function and types in the name of their custom game state (derived from the parent interface) as the template input. The engine will then construct an object of the custom game state type and store it inside its game state pointer after it cleans up the previously-active game state. How that's done will become clear after the game state methods and their application are explained in detail

- Game state entry (OnEntry) - this method gets called once when the program transitions INTO the current game state. This is where the game state object should load all of the resources that it needs (it acts as a class constructor). This method also provides a point for executing any game logic that has to run exactly once per game state activation (like getting the results of some analysis or storing some data to a file)
- Input handling (HandleInput) - this method gets called once per input event that has been detected for the current frame. The engine will feed the raw event data into this function, and the implementation of this method is responsible for processing it an formatting it into useful information for its respective game state. The system is designed this way, so each game state has access to all input happening in the system and has the option to decide which input to process and which to ignore
- Updating (Update) - this method gets called once per frame and the engine feeds the calculated delta time into it. This is the point for performing automated updates on any in-game components (like animating the movement of a sprite on the screen or incrementing a timer)
- Render (Render) - This is the point when the in-game elements that belong to the current game state should be drawn to the screen and it also gets called once per frame. Rendering functionality shouldn't be called anywhere else in the game state methods because the engine has a dedicated place to call this Render method. It might perform some rendering optimizations earlier inside the game loop before deciding whether to call the rendering logic of the current state or not. Not following this rule might result in broken visual elements and/or unoptimized rendering
- Game state exit (OnExit) - this method gets called once when the program transitions OUT OF the current game state. This is the point where the current state should execute any clean-up/information storing logic (like saving data to a global variable or a file) before getting destroyed automatically by the engine. If the deallocation of some resources cannot be done automatically by the C++ default destructor, it should also happen here, so no memory leaks after exiting the current game state

When having these methods available, the transition between game states becomes straightforward. When the transition function is called, the engine calls the 'OnExit' function of the current game state, destroys it and constructs the new game state in its place and calls the 'OnEntry' function of the new active game state.

The engine also supports a modification of the game state class, called an overlay state. The overlay states are implemented through the same parent interface logic, but have some key differences.
- An overlay state is a game state that (partially) covers another underlying game state (that could be a pause menu or an animated transition). This is useful when you want to return back to the underlying game state and find it exactly as you left it, without having do store its data, destroy it, re-construct it and re-load the saved data
- The overlay states are designed to be part of a sequence of other overlay states (so multiple overlays can be active at the same time)
- They are rendered from bottom-to-top (the top-most overlay state occludes the rest)
- Updates from the engine are passed only to the top-most overlay state (both for input and delta time). Because of that, the states that are below the highest state appear frozen on the screen until the highest state is deactivated
- An overlay state ignores input events by default (useful for animated transitions for example), but implementations of the interface class are allowed to override that behavior
- The overlay state interface contains a flag for rendering optimization that should be toggled on when the overlay state covers the entire screen. This way, the engine will disable the rendering of game states that are below the overlay state that has this flag toggled on (further overlay states on top of it will be rendered normally)

### The Game
The slot game features 3 distinct game modes: regular, extra reel and free spins. The game logic also contains global data structures that store and synchronize the game data between different game states, since no singular game state may keep its data while inactive. This "shared" data is the essence of the game and the different game states serve as ways to modify it.
There is an additional inheritance layer between the engine game state interface and the final game states called the Master State layer. States of this layer implement some common functionality which makes the implementation of the final game states (which there are a lot of) faster and more secure.
Each of the game components will be explained in detail in the following sections.

### Game States Sequences
Regular mode:
- Betting - the player starts in the Betting state. This is when they can adjust their bet amount and enter the Reeling state.
- Reeling - this is the state when the reels run their spinning animation and automatically transitions into the Results state
- Results - this is when the results of the reeling are analyzed and based on them, the player might enter other game modes: if 2 scatter symbols are found, the player enter Extra Reel mode, if 3 or more scatters are found, the player enter Free Spins mode. If the player has to stay in Regular mode, they can choose if they want to return to the Betting state or spin the reels again directly by entering the Reeling state

Extra Reel mode:
- Transition in - the grid gets resized and an additional reel gets added on its right side. Then the transition is complete, the program transitions automatically into the Extra Reel Reeling state
- Extra Reel Reeling - the additional reel that has been added gets spun while the rest of the grid remains static. When the reeling is finished, the program transitions automatically into Extra Reel Results
- Extra Reel Results - the state of the grid is analyzed in reverse (from right to left) and it is determined if the player should receive any rewards from that reeling. If a 3-rd scatter symbol is found (on top of the 2 needed to enter the Extra Reel mode), the player will enter Free Spins mode after exiting the Extra Reel mode. The player may then enter the Transition out of the Extra Reel mode
- Transition out - the additional reel gets removed and the grid gets resized back to its original size. If the player has found the 3-rd scatter during the Extra Reel Reeling, the program will automatically start the Free Spins Transition In, otherwise the player will enter the Regular Betting state after the transition is finished

Free Spins mode:
- Transition in - two wooden doors cover the screen and a prompt is displayed, saying that the player is entering Free Spins Mode. When the doors are closed, the program switches the main game state from whatever it was when the transition started to Free Spins Entry mode. The prompt then disappears and the doors open automatically
- Free Spins Entry - the user gets prompted to start spinning the reels for free and is shown the amount of free spins that they have available. When they do that, the program transitions into Free Spins Reeling
- Free Spins Reeling - the reels spin the same way as they do in Regular mode, only difference is that entering this mode consumes one of the free spins available. After the reeling is finished, the program transitions into Free Spins Results automatically
- Free Spins Results - the outcome of the grid gets analyzed the same way as in the Regular mode. In Free Spins however, any reward achieved by the player is multiplied by 3. If there are any free spins left, the player may enter Free Spins Reeling again, otherwise the Free Spins Transition Out will start
- Transition Out - the wooden doors will cover the screen again and a prompt will be displayed, showing the total amount of credits won during the Free Spins mode. When they are closed, the program will transition into the Regular Betting state. Afterwards, the prompt will disappear and the doors will open

Debug mode - this is a special mode that allows the player to choose the outcome of their next reeling. During the debug mode, a separate game grid that contains only empty cells and a cursor are shown to the player. The grids gets updated dynamically based on the game mode that the Debug mode was entered from and follows the rules that apply to that mode (no more than 1 scatter symbol may show on a single reel, no wild symbol may show in the first reel of the grid and so on).
- Debug Pick Cell - during this mode, the player may move the cursor across the grid with the arrow keys and select a cell which content they wish to modify. They pick it by pressing 'Enter' which transitions them into the Debug Set Cell mode
- Debug Set Cell - by using the up and down arrow keys, the player may choose which content will the selected cell contain after the next reeling is finished. Any cell that is left untouched (with an empty content) will get a random symbol generated in its place during the next reeling

### Shared Game Components
The game grid:
- The grid manages 5 reels, each of which contain 3 visible cells and 1 hidden (the hidden cell is needed for the spinning animation). Each reel can animate itself and stores data about its own position in the grid and the contents of its cells. The reel is responsible for generating random content inside each cell when reeling (there are some d) The grid orders the reels to start and stop their animations at a certain times, stores the state of the entire grid and is able to export it in an useful format for the rest of the program. The grid also stores useful meta-data about itself (like cell size, row and column amount etc.) that can be used for calculating the screen coordinates of its elements by other parts of the program.

The analyzer:
- The analyzer's function is to take the grid state and determine if any reward has been achieved. It defines the paylines of the game and the paytables for each symbol. The analyser "traverses" the grid state for each line and gathers the symbols that are located on it, it then counts up how many matching symbols there are and determines the reward that the player should receive based on that. The analyzer also outputs which lines have gotten a reward for this analyzer session, so they can be displayed on the screen.

The common manager
- The common manager is a global data structure that stores the current state of the game as a whole: the available amount of credits, the bet amount, the current game mode and so on. The common manager is also responsible for writing the game data to a save file when the user quits the game and loading data from the save file when the game is opened again. This way, the progress isn't lost between game sessions.

The input manager:
- The input manager is responsible for getting the raw input data from the engine and formatting it in a useful way for the remaining parts of the program. It maintains a set of bindings (which are pairs of virtual keycodes and logical keycodes) and updates the state of each supported button with each input event. At any point, it is able to provide information if a certain button has been pressed/released at a certain point in time.

The debug manager:
- The debug manager has a similar purpose as the Common manager, but it's specifically designed to support the debug mode, which will be explained in greater detail later

### Master States
There are 3 master states in this game: Master reeling, Master results and Master door transition. These states group common functionality of the respective game state sub-categories and implement them while allowing each game state that derives from them to override that functionality if needed

The master reeling state:
- The master reeling state orders the game grid to prepare its reels for reeling and sets the initial value of an internal timer. It then starts the timer and spins the reels until it runs out.

The master results state:
- The master results state orders the program to run an analyzer session and calculates the reward that the player should receive. The master results state also implements the logic for displaying the win to the player: if one or more winning lines exist, they get displayed in a looping sequence; if 2 or more scatter symbols are found, they get flashing colorful borders; the win amount gets animated as a quickly-rising number. The master results state also implements result logging. It maintains a log file, where the results form the last 10 reel spins are stored.

The master door transition state:
- This master state groups common logic for the wooden door transitions of the Free Spins modes (the door-closing animation and the underlying transition event)

### Debug Mode
The debug mode deserves more attention, since it's very different from the remaining modes of the game. It has its own game grid (debug grid) and is able to export its state as a debug configuration to the actual game grid.
- The debug manager - this is a global data structure that stores the state of the debug grid and automatically toggles certain flags on if a custom debug configuration is available. It is responsible for exporting that information to the game grid
- Data transfer to the regular grid - the way that the grid determines which outcome the reels will show after the reeling is by generating it before the reeling animation starts. By default, a random content gets assigned to each cell, and then the reeling starts (the reeling animation is configured in such a way that the last 3 symbols that will come out for each reel will always be the ones that were generated before the reeling started). With this setup, the grid is able to take input from the debug manager and replace the content in the desired cells with the desired content if a debug configuration is available (if a cell from the debug grid has empty content, the regular grid will generate random content for it and that will become the result when the reeling finishes).
- The debug grid follows the rules of the game and will not allow picking symbols that can't be shown in the cell that is currently selected. This way it is ensured that the program will not run into any errors when the configured result eventually shows up
- When picking scatter symbols in debug mode the grid updates dynamically. If 2 scatters are selected inside the regular 5 reels of the grid, a 6th reel will show up, allowing the player to set the outcome of the extra reel that will show up and spin automatically. If a scatter is selected in the 6th reel, the system will not allow the player to select more scatters in the 5 regular reels (drawing more than 2 directly will send the player straight to Free Spins mode and not Extra Reel). If any scatters are selected in the debug grid, no other cells may get scatters during the random content generation of the regular grid (drawing additional scatters may change the desired outcome of the reeling)

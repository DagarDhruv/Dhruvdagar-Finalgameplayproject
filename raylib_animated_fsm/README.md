# Animated FSM Command Pattern StarterKit Guide <a name="animated-fsm-command-pattern-starterkit-guide"></a>

A comprehensive _StarterKit_ for implementing the _Command design pattern_ and _Animated Finite State Machine (FSM)_ in C using Raylib. This kit combines state management, command patterns, and sprite-based animations to create dynamic game entities and NPCs.

## Table of Contents

- [Animated FSM Command Pattern StarterKit Guide](#animated-fsm-command-pattern-starterkit-guide)
- [Overview](#overview)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Animation System](#animation-system)
  - [Animation Components](#animation-components)
  - [Animation States](#animation-states)
- [Implementation Guide](#implementation-guide)
  - [Basic Usage Example](#basic-usage-example)
  - [State Transitions](#state-transitions)
- [Build Configuration](#build-configuration)
- [Resources](#resources)
- [Support](#support)

## Overview <a name="overview"></a>

This starter kit provides:

- Complete Command pattern implementation with animation support
- Animated Finite State Machine for entity management
- State transition validation system
- Sprite sheet animation system
- Multiple NPC types with unique behaviors (could be added PollAI)
- Mediator pattern for component decoupling Command to FSM
- Flexible input handling system
- Game loop integration
- State-aware command execution
- Basic shape-based rendering
- Simple collision detection system
- NPC behavior system (PollAI)

## Architecture <a name="architecture"></a>

The system consists of ten main components:

1. **Animation**
   - Manages sprite sheets and frame timing
   - Handles animation state transitions
   - Controls frame sequences
   - Manages animation speeds

2. **Player**

   - Maintains position and velocity
   - Contains reference to current FSM state
   - Manages animation states
   - Processes command results
   - Handles collision responses

3. **NPC**

   - State-based behaviors
   - Unique animation sets
   - Collision detection
   - AI behavior pattern (basic PollAI)

4. **Collision System**

   - Circle vs Circle collision
   - AABB vs AABB collision
   - Circle vs AABB collision
   - Basic collision response

5. **Finite State Machine**

   - State enumeration and tracking
   - State transition logic
   - Animation state mapping
   - Controls entity behavior

6. **Command System**

   - Command type enumeration
   - State-aware command execution

7. **Mediator**

   - Decouples components
   - Manages communication
   - Handles command routing
   - Updates entities

8. **Input Manager**

   - Keyboard input handling
   - Controller support
   - Command mapping

9. **Game System**
    - Initialization
    - Update loop
    - State-based rendering
    - Animation synchronization

## Project Structure <a name="project-structure"></a>

```
animated_fsm_starter/
├── lib/
│   ├── raylib_starter/       # Raylib starter submodule
│   └── cute_headers/         # cute_c2 collision submodule
├── include/
│   ├── player.h              # Player structure and behavior
│   ├── npc.h                 # NPC structure and behavior
│   ├── fsm.h                 # FSM structures and state declarations
│   ├── command.h             # Command structures and functions
│   ├── mediator.h            # Mediator pattern implementation
│   ├── input_manager.h       # Input handling system
│   ├── animation.h           # Animation system components
│   └── game.h                # Game system header
├── src/
│   ├── player.c              # Player implementation
│   ├── npc.c                 # NPC implementation
│   ├── fsm.c                 # FSM implementation
│   ├── command.c             # Command implementations
│   ├── mediator.c            # Mediator implementation
│   ├── input_manager.c       # Input system implementation
│   ├── animation.c           # Animation system implementation
│   ├── game.c                # Game system implementation
│   └── main.c                # Entry point
├── assets/
│   ├── player.png            # Player sprite sheets
│   └── npc.png               # NPC sprite sheets
├── Makefile                  # Build configuration
└── README.md                 # This documentation
```

## Animation System <a name="animation-system"></a>

### Animation Components <a name="animation-components"></a>

```c
typedef struct
{
    Texture2D texture;   // Animated Sprite Sheet Texture
    Rectangle *frames;   // Array of frames (rectangles)
    int currentFrame;    // Current frame index
    int frameCount;      // Total number of frames
    float frameDuration; // Duration of each frame
    float frameTimer;    // Timer to track frame duration
    bool active;         // Is the animation active?
    bool loop;           // Should the animation loop?
} AnimationData;
```

### Animation States <a name="animation-states"></a>

```c

#ifndef ANIMATION_H
#define ANIMATION_H

#include <raylib.h>

typedef struct
{
    Texture2D texture;   // Animated Sprite Sheet Texture
    Rectangle *frames;   // Array of frames (rectangles)
    int currentFrame;    // Current frame index
    int frameCount;      // Total number of frames
    float frameDuration; // Duration of each frame
    float frameTimer;    // Timer to track frame duration
    bool active;         // Is the animation active?
    bool loop;           // Should the animation loop?
} AnimationData;

// Init Animation
void InitAnimation(AnimationData *animationData, Texture2D texture, Rectangle *frames, int frameCount, float frameDuration, bool loop);

// Update Animation
void UpdateAnimation(AnimationData *animationData);

// Render Animation
void RenderAnimation(const AnimationData *animationData, Vector2 position, Color tint);

#endif // ANIMATION_H

// Within Player (Init)

    // ---- STATE_IDLE state configuration ----
    // Define valid transitions from STATE_IDLE
    State idleValidTransitions[] = {STATE_WALKING, STATE_ATTACKING, STATE_SHIELD, STATE_DEAD};

    // Set up the state configuration for STATE_IDLE
    obj->stateConfigs[STATE_IDLE].name = "Player_Idle";
    obj->stateConfigs[STATE_IDLE].HandleEvent = PlayerIdleHandleEvent;
    obj->stateConfigs[STATE_IDLE].Entry = PlayerEnterIdle;
    obj->stateConfigs[STATE_IDLE].Update = PlayerUpdateIdle;
    obj->stateConfigs[STATE_IDLE].Exit = PlayerExitIdle;

    // Configure valid transitions for STATE_IDLE
    StateTransitions(&obj->stateConfigs[STATE_IDLE], idleValidTransitions, sizeof(idleValidTransitions) / sizeof(State));



void PlayerEnterIdle(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> ENTER -> Idle\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

    if (player->base.previousState != player->base.currentState && player->base.currentState == STATE_IDLE)
    {
        Rectangle idle[6] = {
            {0, 1472, 64, 64},
            {64, 1472, 64, 64},
            {0, 1536, 64, 64},
            {64, 1536, 64, 64},
            {0, 1600, 64, 64},
            {64, 1600, 64, 64}};

        // Initialize the idle animation frames and play it
        InitGameObjectAnimation(&player->base, idle, 6, 0.2f);
    }
}
```

## Implementation Guide <a name="implementation-guide"></a>

### Basic Usage Example <a name="basic-usage-example"></a>

```c
// Initialize player with animations
// Load player texture
    Texture2D playerTexture = LoadTexture("./assets/player_sprite_sheet.png");

    InitGameObject(&player->base,
                   name,                                                         // Name
                   (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, // Position
                   (Vector2){0, 0},                                              // Velocity
                   STATE_IDLE,                                                   // Initial State
                   GREEN,                                                        // Player Color
                   (c2Circle){                                                   // cute_c2 Circle Collider
                              .p = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
                              .r = 10},
                   (c2AABB){// AABB Collider for boundary checks
                            .min = {GetScreenWidth() / 2.0f - 10, GetScreenHeight() / 2.0f - 10},
                            .max = {GetScreenWidth() / 2.0f + 10, GetScreenHeight() / 2.0f + 10}},
                   playerTexture,
                   100 // Initial Health
    );

    // Player Specific Data
    player->stamina = 100.0f;
    player->mana = 100.0f;

    // Init the Player FSM
    InitPlayerFSM(&player->base);

    if (player->base.currentState == STATE_IDLE)
    {
        // Initialize the idle animation immediately
        PlayerEnterIdle(&player->base); // Trigger idle animation at initialization
    }

// Somewhere in Update Method
// Poll input from the user and execute the corresponding command
Command command = PollInput();
ExecuteCommand(command, gameData->mediator); // Execute the command via the mediator

// Update the player's state based on its current configuration
UpdateState(&gameData->player->base);

// Somewhere is Draw Method
// Render the player's animation at their current position
RenderAnimation(&gameData->player->base.animation, gameData->player->base.position, WHITE);

```

### State Transitions <a name="state-transitions"></a>

Valid state transitions include:

1. **IDLE -> WALKING**

   - Trigger: Movement commands
   - Animation: Smooth blend from idle to walk cycle

2. **WALKING -> IDLE**

   - Trigger: No movement input
   - Animation: Return to idle stance

3. **Any State -> ATTACK**

   - Trigger: Attack command
   - Animation: Play attack sequence

4. **ATTACK -> IDLE**

   - Trigger: Attack animation complete
   - Animation: Return to previous animation

5. **Any State -> SHIELD**

   - Trigger: Taking damage
   - Animation: Brief damage reaction

6. **Any State -> DIE**
   - Trigger: Health reaches zero
   - Animation: Death sequence

## Build Configuration <a name="build-configuration"></a>

```bash
# Build all targets (desktop)
make all

# Build desktop version
make build

# Run desktop version
make run

# Build web version
make build_web

# Run web version (starts local server)
make run_web

# Clean build files
make clean

# Clean toolchain files
make clean_toolchain

# Build release (desktop)
make CONFIG=release
```

## Resources <a name="resources"></a>

- [Raylib Textures and Sprites Guide](https://www.raylib.com/examples.html)
- [Sprite Sheet Animation Tutorial](https://www.raylib.com/examples/textures/loader.html?name=textures_sprite_button)
- [Animation State Machines in Games](http://gameprogrammingpatterns.com/state.html)
- [Command Pattern in C](https://www.geeksforgeeks.org/command-pattern/)
- [Raylib Starter Kit](https://bitbucket.org/MuddyGames/raylib_starter/src/main/)
- [cute_headers Library](https://github.com/RandyGaul/cute_headers/tree/master)
- [Raylib CheatSheet v5.0 quick reference card](https://www.raylib.com/cheatsheet/cheatsheet.html)

## Support <a name="support"></a>

For questions and support:

- Contact: muddygames

[Back to top](#animated-fsm-command-pattern-starterkit-guide)

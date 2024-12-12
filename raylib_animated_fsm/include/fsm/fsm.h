#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdio.h>

// Include the events header file that defines the 'Event' enum
#include "../include/events/events.h"

// Forward declaration of the GameObject structure
typedef struct GameObject GameObject;

// Define function pointer types for event handling and state management
typedef void (*EventFunction)(GameObject *, Event); // Function type for event handlers
typedef void (*StateFunction)(GameObject *);        // Function type for state entry, update, and exit handlers

// Define an enumeration for different states of the game object
typedef enum
{
    STATE_IDLE,      // Represents the idle state (no action)
    STATE_WALKING,   // Represents the walking state
    STATE_MOVING_UP,    // Represents the state when the player is moving upwards
    STATE_MOVING_DOWN,  // Represents the state when the player is moving downwards
    STATE_MOVING_LEFT,  // Represents the state when the player is moving to the left
    STATE_MOVING_RIGHT, // Represents the state when the player is moving to the right
    STATE_MOVING_UP_LEFT,
    STATE_MOVING_UP_RIGHT,
    STATE_MOVING_DOWN_LEFT,
    STATE_MOVING_DOWN_RIGHT,
    STATE_ATTACKING, // Represents the attacking state
    STATE_SHIELD,    // Represents the shield state (defensive posture)
    STATE_DEAD,      // Represents the dead state (game over or defeated)
    STATE_RESPAWN,   // Represents the respawn state (respawns the player)
    STATE_COLLISION, // Represents the state when a collision is detected (e.g., with an enemy or obstacle)
    STATE_COUNT,      // Represents the total number of states (for counting purposes)
} State;             // Define 'State' as the type of the enum

/**
STATES TO BE IMPLEMENTED
Define an enumeration for different states of the game object
typedef enum
{
    STATE_MOVING_UP,    // Represents the state when the player is moving upwards
    STATE_MOVING_DOWN,  // Represents the state when the player is moving downwards
    STATE_MOVING_LEFT,  // Represents the state when the player is moving to the left
    STATE_MOVING_RIGHT, // Represents the state when the player is moving to the right
    STATE_FIRING,       // Represents the state when the player is firing a weapon or projectile
    STATE_COUNT         // Represents the total number of states (used for validation or array sizing)
} State;                // Define 'State' as the type of the enum, making it easier to refer to in the code
*/

// Define a configuration structure for each state of the GameObject
typedef struct StateConfig
{
    const char *name;          // The name of the state (e.g., "Idle", "Walking")
    EventFunction HandleEvent; // Pointer to the function that handles events in this state
    StateFunction Entry;       // Pointer to the function that is called when entering this state
    StateFunction Update;      // Pointer to the function that is called to update the state
    StateFunction Exit;        // Pointer to the function that is called when exiting this state
    State *nextStates;         // Array of possible next states (state transitions)
    int nextStatesCount;       // Number of possible next states
} StateConfig;                 // Define 'StateConfig' as a structure that holds all state-related configurations

// Handles an event for the given game object, triggering changes in state
void HandleEvent(GameObject *obj, Event event);

// Checks if the game object can enter a new state based on the current context
bool CanEnterState(GameObject *obj, State newState);

// Changes the state of the game object to the new state if possible
bool ChangeState(GameObject *obj, State newState);

// Updates the current state of the game object (for example, animations, actions)
void UpdateState(GameObject *obj);

// Function to initialize valid state transitions
void StateTransitions(StateConfig *stateConfig, State *transitions, int count);

// Function to print each state configuration
void PrintStateConfigs(StateConfig *stateConfigs, int stateCount);

#endif

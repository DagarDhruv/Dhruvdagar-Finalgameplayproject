#include "../include/fsm/fsm.h"
#include "../include/gameobjects/gameobject.h"

/**
 * HandleEvent - Handles an event for a given game object based on its current state.
 *
 * This function checks the current state of the game object and, if an event handler
 * (HandleEvent) is defined for the current state, it calls that function to handle the event.
 *
 * @obj:   A pointer to the GameObject that is receiving the event.
 * @event: The event to be handled (such as a user input, time-based event, etc.).
 */
void HandleEvent(GameObject *obj, Event event)
{
    // Get the state configuration for the current state of the object
    StateConfig *config = &obj->stateConfigs[obj->currentState];

    // If a HandleEvent function is defined for this state, call it
    if (config->HandleEvent)
    {
        config->HandleEvent(obj, event); // Call the state's event handler
    }
}

/**
 * UpdateState - Updates the game object's state, executing any behavior defined for the current state.
 *
 * This function checks if the current state has an update function defined, and if so, it calls that
 * function to perform any state-specific actions (e.g., animation updates, state-based actions).
 *
 * @obj: A pointer to the GameObject whose state needs to be updated.
 */
void UpdateState(GameObject *obj)
{
    // Get the configuration for the current state
    StateConfig *config = &obj->stateConfigs[obj->currentState];

    // If an update function is defined for the current state, call it
    if (config->Update)
    {
        config->Update(obj); // Perform the update for the current state (e.g., animation, actions)
    }
}

/**
 * CanEnterState - Checks if the game object can transition to a new state.
 *
 * This function checks whether transitioning from the current state to the specified `newState` is valid
 * by verifying if `newState` is listed in the `nextStates` array of the current state's configuration.
 *
 * @obj:      A pointer to the GameObject whose state transitions need to be validated.
 * @newState: The target state to which the object wants to transition.
 *
 * @return: Returns true if the transition is valid, false otherwise.
 */
bool CanEnterState(GameObject *obj, State newState)
{
    // Get the current state configuration
    StateConfig *currentConfig = &obj->stateConfigs[obj->currentState];

    // Loop through the possible next states and check if newState is valid
    for (int i = 0; i < currentConfig->nextStatesCount; i++)
    {
        if (currentConfig->nextStates[i] == newState)
            return true; // Valid transition found
    }
    return false; // No valid transition found
}

/**
 * ChangeState - Attempts to change the game object's state if the transition is valid.
 *
 * This function first checks if the transition to the `newState` is valid by calling `CanEnterState`.
 * If the transition is valid, it will call exit functions (if defined) for the current state, update the
 * object's state, and call entry functions (if defined) for the new state.
 *
 * @obj:      A pointer to the GameObject whose state is being changed.
 * @newState: The state to which the game object will transition.
 *
 * @return: Returns true if the transition is successful, false if the transition is invalid.
 */
bool ChangeState(GameObject *obj, State newState)
{
    // Check if the state transition is valid
    if (!CanEnterState(obj, newState))
    {
        // If the transition is not valid, print an error and return false
        printf("Invalid state transition from %s to %s\n",
               obj->stateConfigs[obj->currentState].name,
               obj->stateConfigs[newState].name);
        return false; // Transition failed
    }

    // Get the configuration of the current state and the new state
    StateConfig *currentConfig = &obj->stateConfigs[obj->currentState];
    StateConfig *newConfig = &obj->stateConfigs[newState];

    // If the current state has an exit function defined, call it
    if (currentConfig->Exit)
        currentConfig->Exit(obj);

    // Update the object's previous and current state
    obj->previousState = obj->currentState;
    obj->currentState = newState;

    // If the new state has an entry function defined, call it
    if (newConfig->Entry)
        newConfig->Entry(obj);

    return true; // State transition successful
}

/**
 * StateTransitions - Initializes the valid state transitions for a specific state.
 *
 * This function allocates memory for the `nextStates` array, which holds all valid states that can be
 * transitioned to from the given state. It copies the provided `transitions` array into the `nextStates`
 * array and sets the count of possible next states.
 *
 * @stateConfig: A pointer to the StateConfig object for the specific state being configured.
 * @transitions: An array of valid state transitions from the given state.
 * @stateCount:  The number of states in the `transitions` array.
 */
void StateTransitions(StateConfig *stateConfig, State *transitions, int stateCount)
{
    // Allocate memory for the next states array based on the given count
    stateConfig->nextStates = (State *)malloc(sizeof(State) * stateCount);
    if (!stateConfig->nextStates)
    {
        // If memory allocation fails, print an error and exit
        fprintf(stderr, "Failed to allocate state transitions\n");
        exit(1); // Exit the program if allocation fails
    }

    // Copy the transitions array into the stateConfig's nextStates array
    memcpy(stateConfig->nextStates, transitions, sizeof(State) * stateCount);
    stateConfig->nextStatesCount = stateCount; // Set the count of next states
}

/**
 * PrintStateConfigs - Prints detailed information about the state configurations.
 *
 * This function prints the configuration details for each state in the system. It shows the state name,
 * whether event handlers (`HandleEvent`), entry, update, and exit functions are defined, as well as the
 * valid state transitions.
 *
 * @stateConfigs: An array of `StateConfig` objects that describe all the states in the system.
 * @stateCount:   The number of states in the `stateConfigs` array.
 */
void PrintStateConfigs(StateConfig *stateConfigs, int stateCount)
{
    // Loop through each state configuration and print its details
    for (int i = 0; i < stateCount; i++)
    {
        StateConfig *config = &stateConfigs[i];

        // Only print if the state is properly configured (i.e., has a name and event handler)
        if (config->name == NULL || config->HandleEvent == NULL)
        {
            continue; // Skip printing incomplete or improperly configured states
        }

        // Print the state name and availability of its handler functions
        printf("State: %s\n", config->name);
        printf("\tHandleEvent: %s\n", config->HandleEvent ? "Defined" : "NULL");
        printf("\tEntry: %s\n", config->Entry ? "Defined" : "NULL");
        printf("\tUpdate: %s\n", config->Update ? "Defined" : "NULL");
        printf("\tExit: %s\n", config->Exit ? "Defined" : "NULL");

        // Print the list of valid next states
        printf("\tNext States: [");
        for (int j = 0; j < config->nextStatesCount; j++)
        {
            if (j > 0)
            {
                printf(", ");
            }
            printf("%d", config->nextStates[j]);
        }
        printf("]\n");
        printf("\tNext States Count: %d\n", config->nextStatesCount);
    }
}

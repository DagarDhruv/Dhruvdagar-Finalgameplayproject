#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/utils/mediator.h"

/**
 * CreateMediator - Creates and initializes a new mediator instance.
 *
 * @obj: The GameObject that the mediator will control and communicate with.
 *
 * This function allocates memory for a new Mediator object and associates it
 * with the provided GameObject. The Mediator serves as an intermediary, enabling
 * communication between the GameObject and external commands, while keeping
 * the GameObject decoupled from direct command handling. The mediator facilitates
 * the triggering of events and actions, such as movement, attacks, or state changes,
 * through its interaction with the GameObject's Finite State Machine (FSM).
 *
 * The Mediator Pattern is used here to centralize communication and control
 * between game objects, allowing for cleaner and more maintainable code.
 *
 * For a good explanation on the Mediator Pattern, see:
 * https://www.geeksforgeeks.org/mediator-design-pattern/
 *
 * Return: A pointer to the newly created Mediator instance, or NULL if memory
 *         allocation fails.
 */
Mediator *CreateMediator(GameObject *obj)
{
    Mediator *mediator = (Mediator *)malloc(sizeof(Mediator));
    if (mediator == NULL)
    {
        return NULL;
    }
    mediator->obj = obj;
    return mediator;
}

/**
 * MediatorExecuteCommand - Executes a command through the mediator and interacts with the GameObject's FSM.
 *
 * @command:  The command to be executed, which determines the event to trigger on the GameObject.
 * @mediator: A pointer to the Mediator instance that handles the command and facilitates interaction with the FSM.
 *
 * This function processes the provided command, which corresponds to an action or state change.
 * It triggers the appropriate event on the GameObject associated with the mediator, effectively interacting
 * with the GameObject's Finite State Machine (FSM). The FSM processes these events (e.g., MOVE, ATTACK, DIE, etc.)
 * and transitions the GameObject between states or executes actions based on the current state.
 *
 * Commands like movement, firing, and collisions are mapped to specific events, and the FSM determines how
 * the GameObject responds to those events, including transitioning to different states or performing specific actions.
 */
void MediatorExecuteCommand(Command command, Mediator *mediator)
{
    if (!mediator || !mediator->obj)
    {
        printf("Error: Mediator or Mediator's obj is NULL\n");
        return;
    }

    switch (command)
    {
        case COMMAND_NONE:
            HandleEvent(mediator->obj, EVENT_NONE);
            break;
        case COMMAND_MOVE_UP:
            HandleEvent(mediator->obj, EVENT_MOVE_UP);
            break;
        case COMMAND_MOVE_DOWN:
            HandleEvent(mediator->obj, EVENT_MOVE_DOWN);
            break;
        case COMMAND_MOVE_LEFT:
            HandleEvent(mediator->obj, EVENT_MOVE_LEFT);
            break;
        case COMMAND_MOVE_RIGHT:
            HandleEvent(mediator->obj, EVENT_MOVE_RIGHT);
            break;
        case COMMAND_ATTACK:
            HandleEvent(mediator->obj, EVENT_ATTACK);
            break;
        case COMMAND_COLLISION_START:
            HandleEvent(mediator->obj, EVENT_DIE);
            break;
        case COMMAND_COLLISION_END:
            HandleEvent(mediator->obj, EVENT_RESPAWN);
            break;
        default:
            break;
    }
}

/**
 * DeleteMediator - Cleans up the mediator instance and frees allocated memory.
 *
 * @mediator: A pointer to the Mediator instance that needs to be deleted.
 *
 * This function frees the memory allocated for the mediator object, ensuring
 * that resources are properly released when the mediator is no longer needed.
 */
void DeleteMediator(Mediator *mediator)
{
    if (mediator)
    {
        free(mediator);
    }
}

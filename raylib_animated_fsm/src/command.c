#include <stdbool.h>

#include "../include/command/command.h"
#include "../include/fsm/fsm.h"
#include "../include/utils/mediator.h"

/**
 * ExecuteCommand - Executes a given command by interacting with the Mediator.
 *
 * This function is responsible for triggering the execution of a command. It serves
 * as an intermediary between the `Command` and the `FSM` (Finite State Machine), by
 * using the `Mediator`. The mediator handles the communication and ensures that the
 * correct action is performed based on the command issued.
 *
 * @command:   The command to be executed.
 * @mediator: A pointer to the `Mediator` object, which coordinates between the
 *            command and the FSM.
 *
 * This function delegates the execution of the command to the `MediatorExecuteCommand`
 * function, which will handle the actual logic of applying the command in the context
 * of the FSM.
 */
void ExecuteCommand(Command command, Mediator *mediator)
{
    // Delegate the execution of the command to the Mediator.
    // The Mediator will process the command and interact with the FSM.
    MediatorExecuteCommand(command, mediator);
}

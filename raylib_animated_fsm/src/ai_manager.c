#include <stdlib.h>
#include <time.h>

#include "../include/command/command.h"
#include "../include/utils/ai_manager.h"

/**
 * InitAIManager - Initializes the AI manager and sets up any required resources.
 *
 * This function is responsible for preparing the AI system for use. In this case, the
 * implementation is a placeholder and does not perform any specific initialization
 * tasks. Additional initialization steps can be added if needed in the future.
 */
void InitAIManager()
{
    // Initialize AI
}

/**
 * PollAI - Retrieves a random command from the AI.
 *
 * This function simulates AI behavior by returning a random command. It uses the
 * `rand()` function to select a command from the available pool of commands, with
 * the total number of commands being defined by `COMMAND_COUNT`.
 *
 * @return: A randomly chosen Command value from the range [0, COMMAND_COUNT-1].
 */
Command PollAI()
{
    // Return a random command
    return (Command)(rand() % COMMAND_COUNT);
}

/**
 * ExitAIManager - Cleans up any resources allocated by the AI manager.
 *
 * This function is responsible for deallocating or cleaning up any resources used
 * by the AI manager. Currently, it is a placeholder since there are no resources
 * to clean up, but it can be expanded in the future if necessary.
 */
void ExitAIManager()
{
    // Cleanup resources if needed - Currently a placeholder.
    // Add any necessary cleanup logic here (e.g., freeing memory, closing files).
}

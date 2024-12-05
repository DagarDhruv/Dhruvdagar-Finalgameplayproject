#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "../include/command/command.h"

void InitInputManager();
Command PollInput();
void ExitInputManager();

#endif // INPUT_MANAGER_H
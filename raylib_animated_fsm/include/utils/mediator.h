#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <stdbool.h>

#include "../include/command/command.h"
#include "../include/gameobjects/gameobject.h"

// Define the Mediator structure
typedef struct Mediator
{
    GameObject *obj;
} Mediator;

// Function to create a mediator instance
Mediator *CreateMediator(GameObject *obj);

// Execute Command
void MediatorExecuteCommand(Command command, Mediator *mediator);

// Cleanup Mediator
void DeleteMediator(Mediator *mediator);

#endif // MEDIATOR_H
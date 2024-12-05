#ifndef NPC_H
#define NPC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Include the header for the base game object
#include "gameobject.h"

// Define the NPC structure that extends GameObject with an additional aggression property
typedef struct
{
    GameObject base; // The base game object (inherits from GameObject)
    int aggression;  // The aggression level of the NPC (could affect behavior)
} NPC;

// Initialize a new NPC with a given name (returns a pointer to the NPC)
NPC *InitNPC(const char *name);

// Cleanup NPC
void DeleteNPC(GameObject *obj);

// Initialize NPC-specific states for the given GameObject
void InitNPCFSM(GameObject *obj);

// NPC-specific behaviors for different states

// Handle events in the idle state
void NPCIdleHandleEvent(GameObject *obj, Event event);

// State transition functions for idle state
void NPCEnterIdle(GameObject *obj);
void NPCUpdateIdle(GameObject *obj);
void NPCExitIdle(GameObject *obj);

// Handle events in the attacking state
void NPCAttackingHandleEvent(GameObject *obj, Event event);

// State transition functions for attacking state
void NPCEnterAttacking(GameObject *obj);
void NPCUpdateAttacking(GameObject *obj);
void NPCExitAttacking(GameObject *obj);

// Handle events in the shielding state
void NPCShieldingHandleEvent(GameObject *obj, Event event);

// State transition functions for shielding state
void NPCEnterShielding(GameObject *obj);
void NPCUpdateShielding(GameObject *obj);
void NPCExitShielding(GameObject *obj);

// Handle events in the dead state
void NPCDeadHandleEvent(GameObject *obj, Event event);

// State transition functions for dead state
void NPCEnterDead(GameObject *obj);
void NPCUpdateDead(GameObject *obj);
void NPCExitDead(GameObject *obj);

#endif // NPC_H
#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Include the header for the base game object
#include "gameobject.h"

// Define the Player structure that extends GameObject with additional properties like stamina and mana
typedef struct
{
    GameObject base; // The base game object (inherits from GameObject)
    float stamina;   // The player's stamina (could affect actions like running, attacking)
    float mana;      // The player's mana (could affect magic abilities or special moves)
    float speed;     //speed of the player at which the player moves
    Vector2 spawnPoint;
    int lives;
    Color shieldColor;
    float shieldRadius;
    bool shieldActive;
} Player;

// Initialize a new Player with a given name (returns a pointer to the Player)
Player *InitPlayer(const char *name);

// Cleanup Player
void DeletePlayer(GameObject *obj);

// Initialize the finite state machine (FSM) for the Player (sets up the player's states)
void InitPlayerFSM(GameObject *obj);

// Player-specific behaviors for different states

// Handle events in the idle state (when the player is not performing any action)
void PlayerIdleHandleEvent(GameObject *obj, Event event);

// State transition functions for idle state
void PlayerEnterIdle(GameObject *obj);  // Called when entering the idle state
void PlayerUpdateIdle(GameObject *obj); // Called to update the player's behavior while idle
void PlayerExitIdle(GameObject *obj);   // Called when exiting the idle state

// Handle events in the walking state (when the player is walking)
void PlayerWalkingHandleEvent(GameObject *obj, Event event);

// State transition functions for walking state
void PlayerEnterWalking(GameObject *obj);  // Called when entering the walking state
void PlayerUpdateWalking(GameObject *obj); // Called to update the player's behavior while walking
void PlayerExitWalking(GameObject *obj);   // Called when exiting the walking state

// Handle events in the attacking state (when the player is attacking)
void PlayerAttackingHandleEvent(GameObject *obj, Event event);

// State transition functions for attacking state
void PlayerEnterAttacking(GameObject *obj);  // Called when entering the attacking state
void PlayerUpdateAttacking(GameObject *obj); // Called to update the player's behavior while attacking
void PlayerExitAttacking(GameObject *obj);   // Called when exiting the attacking state


// Handle events in the die state (when the player dies)
void PlayerDieHandleEvent(GameObject *obj, Event event);

// State transition functions for die state
void PlayerEnterDie(GameObject *obj);  // Called when entering the die state
void PlayerUpdateDie(GameObject *obj); // Called to update the player's behavior while dead
void PlayerExitDie(GameObject *obj);   // Called when exiting the dead state

// Handle events in the respawn state (when the player respawns)
void PlayerRespawnHandleEvent(GameObject *obj, Event event);

// State transition functions for respawn state
void PlayerEnterRespawn(GameObject *obj);  // Called when entering the respawn state
void PlayerUpdateRespawn(GameObject *obj); // Called to update the player's behavior while respawning
void PlayerExitRespawn(GameObject *obj);   // Called when exiting the respawn
// state transitions function for player magic
void PlayerEnterShield(GameObject *obj);
void PlayerShieldHandleEvent(GameObject *obj, Event event);
void PlayerUpdateShield(GameObject *obj);
void PlayerExitShield(GameObject *obj);
#endif // PLAYER_H
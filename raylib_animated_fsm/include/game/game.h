#ifndef GAME_H
#define GAME_H

#include "../utils/mediator.h"
#include "../gameobjects/player.h"
#include "../gameobjects/npc.h"
#include "../utils/ai_manager.h"
#include "../utils/input_manager.h"

// Define the GameData struct to store the main game components (player, npc, and mediator)
typedef struct
{
    Player *player;     // Pointer to the Player object
    NPC *npc;           // Pointer to the NPC object
    Mediator *mediator; // Pointer to the Mediator object for managing interactions
                        // Mediator between command and FSM
} GameData;

// Initialises the game components (player, npc, mediator)
void InitGame(GameData *gameData);

// Updates the game state each frame (handles game logic)
void UpdateGame(GameData *gameData);

// Draws or renders the current game state (e.g., player, npc, environment)
void DrawGame(GameData *gameData);

// Closes the game, performing necessary cleanup and freeing resources
void CloseGame(GameData *gameData);

// Frees memory associated with GameData and its components (player, npc, mediator)
void DeleteGameData(GameData *gameData);

#endif // GAME_H
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <raylib.h>

#include "../include/game/game.h"
#include "../include/events/events.h"
#include "../include/fsm/fsm.h"
#include "../include/gameobjects/gameobject.h"
#include "../include/gameobjects/player.h"
#include "../include/gameobjects/npc.h"
#include "../include/utils/mediator.h"
#include "../include/utils/input_manager.h"
#include "../include/utils/ai_manager.h"

// Specific include for build_web
#if defined(WEB_BUILD)
#include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 600;

void GameLoop(GameData *gameData);

int main(void)
{
    // Seed the random number generator once at the start of the program
    srand(time(NULL));

    InitWindow(screenWidth, screenHeight, "Raylib Animated FSM StarterKit GPPI");

    // Create and initialize Game Data
    GameData gameData;

    // Initialise Game
    InitGame(&gameData);

    // For web builds, do not use WindowShouldClose
    // see https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#41-avoid-raylib-whilewindowshouldclose-loop

#if defined(WEB_BUILD)
    emscripten_set_main_loop_arg((void (*)(void *))GameLoop, &gameData, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Call GameLoop
        GameLoop(&gameData);
    }
#endif

    // Free resources
    CloseGame(&gameData);

    CloseWindow();

    return 0;
}

void GameLoop(GameData *gameData)
{
    // Update Game Data
    // Should be outside BeginDrawing(); and EndDrawing();
    UpdateGame(gameData);

    // Draw the Game Objects
    DrawGame(gameData);
}
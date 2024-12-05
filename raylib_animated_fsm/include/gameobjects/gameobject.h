#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdlib.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>

#include "../cute_headers/cute_c2.h"

#include "../include/events/events.h"
#include "../include/fsm/fsm.h"
#include "../include/animation/animation.h"

// Base structure for a game object
typedef struct GameObject
{
    const char *name;    // The name of the game object (e.g., "Player", "Enemy")
    State previousState; // The state the game object was previously in
    State currentState;  // The current state of the game object

    StateConfig *stateConfigs; // Pointer to the array of state configurations for this game object

    // Position Vectors
    Vector2 position; // Gameobjects position in the game world
    Vector2 velocity; // Not implemented : Gameobjects movement velocity

    // Shape Color
    Color color; // Gameobject's color, changes based on currentState

    // Collision components
    c2Circle collider; // Circle collider used for collision detection
    c2AABB bounds;     // Not implemented : Axis-Aligned Bounding Box (AABB) for broad-phase collision detection

    // Sprite Sheet Texture
    Texture2D keyframes;

    // Animation
    AnimationData animation; // Player Animation

    int health; // The health of the game object
} GameObject;

// Initialize a new game object with the given name and default values
void InitGameObject(GameObject *obj,
                    const char *name,
                    Vector2 position,
                    Vector2 velocity,
                    State currentState,
                    Color color,
                    c2Circle collider,
                    c2AABB bounds,
                    Texture2D keyframes,
                    int health);

// Helper function to initialize animation
void InitGameObjectAnimation(GameObject *obj, Rectangle *frames, int frameCount, float speed);

// Check collision
bool CheckCollision(GameObject *lhs, GameObject *rhs);

// Handle Collision
void HandleCollision(GameObject *lhs, GameObject *rhs);

// Delete a game object and free associated memory/resources
void DeleteGameObject(GameObject *obj);

#endif // GAMEOBJECT_H
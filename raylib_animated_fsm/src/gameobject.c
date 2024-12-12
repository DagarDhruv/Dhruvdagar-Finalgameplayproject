#include "../include/gameobjects/gameobject.h"
#include "../include/utils/constants.h"

// Specific define for CUTE_HEADERS, enabling implementation of functions
#define CUTE_C2_IMPLEMENTATION

// Disable warnings for cute_c2
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "../cute_headers/cute_c2.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

/**
 * @brief Initializes a GameObject with default values and assigns a name.
 *
 * This function sets up a new GameObject with the specified properties, such as
 * position, velocity, state, color, collider, texture (keyframes), and health.
 *
 * @param obj The GameObject to be initialized.
 * @param name The name to assign to the GameObject.
 * @param position The initial position of the GameObject (in world coordinates).
 * @param velocity The initial velocity of the GameObject, which determines its movement.
 * @param currentState The initial state of the GameObject (e.g., IDLE, RUNNING).
 * @param color The color used to render the GameObject (e.g., for drawing circles or other shapes).
 * @param collider The collider for the GameObject, used for collision detection (e.g., circular collider).
 * @param bounds The bounding box of the GameObject, used for spatial checks.
 * @param keyframes The texture (sprite sheet) to be used for the GameObject's animations.
 * @param health The initial health value of the GameObject
 *
 * This function sets up all the necessary properties for a GameObject, including
 * its name, position, velocity, current state, health, and other attributes needed
 * for gameplay, such as texture (keyframes), collider, and bounding box.
 */

void InitGameObject(GameObject *obj,
                    const char *name,
                    Vector2 position,
                    Vector2 velocity,
                    State currentState,
                    Color color,
                    c2Circle collider,
                    c2AABB bounds,
                    Texture2D keyframes,
                    int health,
                    float speed)
{
    // Set the GameObject's name
    obj->name = name;

    obj->position = position;
    obj->velocity = velocity;

    // Initialize the previous and current states to currentState (normally IDLE)
    obj->currentState = currentState;
    obj->previousState = STATE_COUNT; // So that at initial load idle animation is loaded

    obj->color = color;
    obj->collider = collider;
    obj->bounds = bounds;
    obj->keyframes = keyframes;
    obj->health = health;
    obj->speed = speed;
}

/**
 * Helper function to initialize animation for the GameObject
 * This function sets up the animation for a GameObject using a texture, frames, and speed.
 *
 * @obj: The GameObject to initialize the animation for.
 * @frames: An array of Rectangle structs defining the frame regions on the sprite sheet.
 * @frameCount: The total number of frames in the animation.
 * @speed: The speed at which the animation should play (frame duration in seconds).
 *
 * This function initializes the animation data for the GameObject, setting up the
 * texture, frames, and speed, and ensures the animation will loop.
 */
void InitGameObjectAnimation(GameObject *obj, Rectangle *frames, int frameCount, float speed)
{
    AnimationData animation;
    InitAnimation(&animation, obj->keyframes, frames, frameCount, speed, true);
    obj->animation = animation;
}

/**
 * CheckCollision - Checks for a collision between the player and an NPC.
 *
 * @player: A pointer to the Player structure, containing the player’s
 *          position and collision circle.
 * @npc:    A pointer to the NPC structure, containing the NPC’s
 *          position and collision circle.
 *
 * This function first checks for a simple circle-to-circle collision using
 * the c2CircletoCircle function. If a collision is detected, it further checks
 * the distance between the player and NPC positions against a collision threshold.
 * This threshold accounts for the total combined radii of both colliders
 * and a buffer (COLLISION_BUFFER) to prevent overly close interactions.
 *
 * Returns true if a collision is detected within the threshold, otherwise false.
 */
bool CheckCollision(GameObject *lhs, GameObject *rhs)
{
    // Perform basic circle-to-circle collision detection
    bool isColliding = c2CircletoCircle(lhs->collider, rhs->collider);

    // If no collision, return false
    if (!isColliding)
        return false;

    // If a collision is detected, calculate the distance between the two colliders' centers
    float distance = Vector2Distance(lhs->position, rhs->position);

    // Calculate the combined radii of both colliders
    float totalRadii = lhs->collider.r + rhs->collider.r;

    // Return true only if the distance is within the collision threshold
    return distance < (totalRadii - COLLISION_BUFFER);
}

/**
 * HandleCollision - Responds to a detected collision between the player and an NPC.
 *
 * @player: A pointer to the Player structure, representing the player character
 *          in the game. This function modifies the player’s health and position.
 * @npc:    A pointer to the NPC structure, representing the non-player character.
 *          This function modifies the NPC's appearance to show collision feedback.
 *
 * This function applies the effects of a collision on the player and NPC.
 * It reduces the player's health by a specified amount and changes the NPC's color
 * to visually indicate the collision. Additionally, the player is pushed back slightly
 * by a vector calculated along the direction from the NPC to the player.
 */
void HandleCollision(GameObject *lhs, GameObject *rhs)
{
    // Decrease the player's health as a result of the collision
    lhs->health -= 5; // Example amount

    // Change NPC color to visually indicate a collision has occurred
    rhs->color = RED;

    // Calculate the vector direction from the NPC to the Player
    Vector2 collisionDirection = Vector2Subtract(lhs->position, rhs->position);

    // Normalize the collision direction to a unit vector
    collisionDirection = Vector2Normalize(collisionDirection);

    // Push the player back slightly along the collision direction
    lhs->position = Vector2Add(lhs->position, Vector2Scale(collisionDirection, COLLISION_PUSH_BACK));
}


/**
 * DeleteGameObject - Frees all dynamically allocated memory associated with a GameObject.
 *
 * This includes memory for state configurations and the GameObject itself.
 * The function ensures that memory is properly freed to avoid memory leaks.
 *
 * @obj: A pointer to the GameObject to be deleted.
 */
void DeleteGameObject(GameObject *obj)
{
    if (obj == NULL)
        return;

    // Check if state configurations exist for this GameObject
    if (obj->stateConfigs)
    {
        // Free each state's nextStates array if it exists
        for (int i = 0; i < STATE_COUNT; i++)
        {
            if (obj->stateConfigs[i].nextStates != NULL)
            {
                free(obj->stateConfigs[i].nextStates);
                obj->stateConfigs[i].nextStates = NULL; // Nullify the pointer after freeing
            }
        }

        // Free state configurations
        free(obj->stateConfigs);
        obj->stateConfigs = NULL; // Nullify after freeing
    }

    // Free the GameObject
    free(obj);
    obj = NULL; // Nullify
}

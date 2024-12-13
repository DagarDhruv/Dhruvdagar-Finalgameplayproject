#include "../include/gameobjects/npc.h"
#include "include/game/game.h"

/**
 * InitNPC - Initializes a new NPC object with a given name.
 *
 * @name: The name of the NPC being initialized.
 *
 * This function allocates memory for the NPC object, initializes the GameObject
 * base structure, and sets the NPC's texture, aggression level, and state
 * machine. It also sets up necessary colliders and starting values for the NPC.
 *
 * Return: A pointer to the initialized NPC object, or NULL if memory allocation
 *         or texture loading fails.
 */
NPC *InitNPC(const char *name)
{
    // Allocate memory for the NPC structure
    NPC *npc = (NPC *)malloc(sizeof(NPC));

    // Check if memory allocation failed
    if (!npc)
    {
        // Print an error message to stderr and terminate the program if allocation fails
        fprintf(stderr, "Failed to allocate NPC\n");
        exit(1);
    }

    // Load player texture
    Texture2D npcTexture = LoadTexture("./assets/npc_sprite_sheet.png");

    // Initialize the base GameObject structure within the NPC with the provided name
    InitGameObject(&npc->base,
                   name,
                   (Vector2){GetScreenWidth() / 2.0f, 100.0f}, // Position
                   (Vector2){0, 0},                            // Velocity
                   STATE_IDLE,                                 // Initial State
                   GREEN,                                      // Player Color
                   (c2Circle){                                 // cute_c2 Circle Collider
                              .p = {GetScreenWidth() / 2.0f, 100.0f},
                              .r = 10},
                   (c2AABB){// AABB Collider for boundary checks
                            .min = {GetScreenWidth() / 2.0f - 10, 100.0f - 10},
                            .max = {GetScreenWidth() / 2.0f + 10, 100.0f + 10}},
                   npcTexture,
                   100, // Initial Health
                   2
    );

    // Set the default aggression level for the NPC
    npc->aggression = 50;

    // Initialize the NPC's finite state machine (FSM) with state configurations
    InitNPCFSM(&npc->base);

    if (npc->base.currentState == STATE_IDLE)
    {
        // Initialize the idle animation immediately
        NPCEnterIdle(&npc->base); // Trigger idle animation at initialization
    }

    // Return a pointer to the initialized NPC object
    return npc;
}

/**
 * DeleteNPC - Frees the memory allocated for the NPC and its associated GameObject.
 *
 * @obj: The GameObject (NPC) to be cleaned up.
 *
 * This function performs the cleanup of the NPC's specific resources and then
 * calls the `DeleteGameObject` function to handle the general cleanup.
 * It is responsible for ensuring no memory leaks related to the NPC.
 */
void DeleteNPC(GameObject *obj)
{
    // Perform any npc-specific cleanup here
    // Cast to NPC if npc-specific cleanup is required
    // NPC *npc = (NPC *)obj;
    // Example of potential cleanup (not implemented here):
    // If the npc is holding a dynamically allocated object, such as a thor hammer:
    // free(npc->holding);
    DeleteGameObject(obj);
}

/**
 * InitNPCFSM - Initializes the Finite State Machine (FSM) for the NPC.
 *
 * @obj: The GameObject (NPC) to initialize the FSM for.
 *
 * This function sets up the state machine for the NPC, allocating memory for
 * state configurations, defining valid state transitions, and associating
 * state handler functions with each state.
 */
void InitNPCFSM(GameObject *obj)
{
    // Allocate memory for the state configurations array with a size for all possible states
    obj->stateConfigs = (StateConfig *)malloc(sizeof(StateConfig) * STATE_COUNT);

    // Check if memory allocation for state configurations failed
    if (!obj->stateConfigs)
    {
        // Print an error message and exit if allocation fails
        fprintf(stderr, "Failed to allocate state configs\n");
        exit(1);
    }

    // ---- STATE_IDLE state configuration ----
    // Define valid transitions from STATE_IDLE
    State idleValidTransitions[] = {STATE_ATTACKING, STATE_SHIELD, STATE_DEAD , STATE_IDLE};

    // Set up the state configuration for STATE_IDLE
    obj->stateConfigs[STATE_IDLE].name = "NPC_Idle";
    obj->stateConfigs[STATE_IDLE].HandleEvent = NPCIdleHandleEvent;
    obj->stateConfigs[STATE_IDLE].Entry = NPCEnterIdle;
    obj->stateConfigs[STATE_IDLE].Update = NPCUpdateIdle;
    obj->stateConfigs[STATE_IDLE].Exit = NPCExitIdle;

    // Configure valid transitions for STATE_IDLE
    StateTransitions(&obj->stateConfigs[STATE_IDLE], idleValidTransitions, sizeof(idleValidTransitions) / sizeof(State));

    // ---- STATE_ATTACKING state configuration ----
    // Define valid transitions from STATE_ATTACKING
    State attackValidTransitions[] = {STATE_IDLE, STATE_SHIELD, STATE_DEAD};

    // Set up the state configuration for STATE_ATTACKING
    obj->stateConfigs[STATE_ATTACKING].name = "NPC_Attacking";
    obj->stateConfigs[STATE_ATTACKING].HandleEvent = NPCAttackingHandleEvent;
    obj->stateConfigs[STATE_ATTACKING].Entry = NPCEnterAttacking;
    obj->stateConfigs[STATE_ATTACKING].Update = NPCUpdateAttacking;
    obj->stateConfigs[STATE_ATTACKING].Exit = NPCExitAttacking;

    // Configure valid transitions for STATE_ATTACKING
    StateTransitions(&obj->stateConfigs[STATE_ATTACKING], attackValidTransitions, sizeof(attackValidTransitions) / sizeof(State));

    // ---- STATE_SHIELD state configuration ----
    // Define valid transitions from STATE_SHIELD
    State sheildingValidTransitions[] = {STATE_IDLE, STATE_ATTACKING, STATE_DEAD};

    // Set up the state configuration for STATE_SHIELD
    obj->stateConfigs[STATE_SHIELD].name = "NPC_Shielding";
    obj->stateConfigs[STATE_SHIELD].HandleEvent = NPCShieldingHandleEvent;
    obj->stateConfigs[STATE_SHIELD].Entry = NPCEnterShielding;
    obj->stateConfigs[STATE_SHIELD].Update = NPCUpdateShielding;
    obj->stateConfigs[STATE_SHIELD].Exit = NPCExitShielding;

    // Configure valid transitions for STATE_SHIELD
    StateTransitions(&obj->stateConfigs[STATE_SHIELD], sheildingValidTransitions, sizeof(sheildingValidTransitions) / sizeof(State));

    // ---- STATE_DEAD state configuration ----
    // Define valid transitions from STATE_DEAD
    State deadValidTransitions[] = {STATE_IDLE}; // Should go to STATE_RESPAWN to keep kit small goes to IDLE

    // Set up the state configuration for STATE_DEAD
    obj->stateConfigs[STATE_DEAD].name = "NPC_Dead";
    obj->stateConfigs[STATE_DEAD].HandleEvent = NPCDeadHandleEvent;
    obj->stateConfigs[STATE_DEAD].Entry = NPCEnterDead;
    obj->stateConfigs[STATE_DEAD].Update = NPCUpdateDead;
    obj->stateConfigs[STATE_DEAD].Exit = NPCExitDead;

    // Configure valid transitions for STATE_DEAD
    StateTransitions(&obj->stateConfigs[STATE_DEAD], deadValidTransitions, sizeof(deadValidTransitions) / sizeof(State));

// For unimplemented states, set them to empty defaults
// Alternatively NPC has its own FSM with only the implemented states
#define EMPTY_STATE_CONFIG \
    (StateConfig){NULL, NULL, NULL, NULL, NULL, NULL, 0}
    obj->stateConfigs[STATE_WALKING] = EMPTY_STATE_CONFIG;
    obj->stateConfigs[STATE_RESPAWN] = EMPTY_STATE_CONFIG;
    obj->stateConfigs[STATE_COLLISION] = EMPTY_STATE_CONFIG;
}

// Handles events for the NPC when in the Idle state
void NPCIdleHandleEvent(GameObject *obj, Event event)
{
    NPC *npc = (NPC *)obj;
    printf("\n%s Idle HandleEvent\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Get distance to player
    Vector2 playerPos = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    float distanceToPlayer = Vector2Distance(obj->position, playerPos);

    // If player moves out of attack range, go back to idle/following
    if (distanceToPlayer > 50.0f) {
        ChangeState(obj, STATE_IDLE);
    }
    switch (event)
    {
    case EVENT_ATTACK:
        // Transition to Attacking state if an attack event is received
        ChangeState(obj, STATE_ATTACKING);
        break;
    case EVENT_DEFEND:
        // Transition to Shielding state if a defend event is received
        ChangeState(obj, STATE_SHIELD);
        break;
    case EVENT_DIE:
        // Transition to Dead state if a die event is received
        ChangeState(obj, STATE_DEAD);
        break;
    // Ignore Events for other cases
    case EVENT_NONE:
    case EVENT_MOVE:
    case EVENT_RESPAWN:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
        case EVENT_MOVE_UP:
            break;
        case EVENT_MOVE_UP_RIGHT:
            break;
        case EVENT_MOVE_UP_LEFT:
            break;
        case EVENT_MOVE_DOWN:
            break;
        case EVENT_MOVE_DOWN_RIGHT:
            break;
        case EVENT_MOVE_DOWN_LEFT:
            break;
        case EVENT_MOVE_LEFT:
            break;
        case EVENT_MOVE_RIGHT:
            break;
        case EVENT_SHIELD:
            break;
    }
}

// Handles events for the NPC when in the Attacking state
void NPCAttackingHandleEvent(GameObject *obj, Event event)
{
    NPC *npc = (NPC *)obj;
    printf("\n%s Attacking HandleEvent\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);

    if (obj->health <= 0) {
        ChangeState(obj, STATE_DEAD);
        return;
    }

        switch (event)
    {
    case EVENT_NONE:
        // Transition back to Idle if no specific event is triggered
        ChangeState(obj, STATE_IDLE);
        break;
    case EVENT_DEFEND:
        // Transition to Shielding state if a defend event is received
        ChangeState(obj, STATE_SHIELD);
        break;
    case EVENT_DIE:
        // Transition to Dead state if a die event is received
        ChangeState(obj, STATE_DEAD);
        break;
    // Ignore Events for other cases
    case EVENT_MOVE:
    case EVENT_ATTACK:
    case EVENT_RESPAWN:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
        case EVENT_MOVE_UP:
            break;
        case EVENT_MOVE_UP_RIGHT:
            break;
        case EVENT_MOVE_UP_LEFT:
            break;
        case EVENT_MOVE_DOWN:
            break;
        case EVENT_MOVE_DOWN_RIGHT:
            break;
        case EVENT_MOVE_DOWN_LEFT:
            break;
        case EVENT_MOVE_LEFT:
            break;
        case EVENT_MOVE_RIGHT:
            break;
        case EVENT_SHIELD:
            break;
    }
}

// Handles events for the NPC when in the Shielding state
void NPCShieldingHandleEvent(GameObject *obj, Event event)
{
    NPC *npc = (NPC *)obj;
    printf("\n%s Shield HandleEvent\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);

    switch (event)
    {
    case EVENT_NONE:
        // Transition back to Idle if no specific event is triggered
        ChangeState(obj, STATE_IDLE);
        break;
    case EVENT_ATTACK:
        // Transition to Attacking state if an attack event is received
        ChangeState(obj, STATE_ATTACKING);
        break;
    case EVENT_DIE:
        // Transition to Dead state if a die event is received
        ChangeState(obj, STATE_DEAD);
        break;
    // Ignore Events for other cases
    case EVENT_RESPAWN:
    case EVENT_MOVE:
    case EVENT_DEFEND:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
        case EVENT_MOVE_UP:
            break;
        case EVENT_MOVE_UP_RIGHT:
            break;
        case EVENT_MOVE_UP_LEFT:
            break;
        case EVENT_MOVE_DOWN:
            break;
        case EVENT_MOVE_DOWN_RIGHT:
            break;
        case EVENT_MOVE_DOWN_LEFT:
            break;
        case EVENT_MOVE_LEFT:
            break;
        case EVENT_MOVE_RIGHT:
            break;
        case EVENT_SHIELD:
            break;
    }
}

// Handles events for the NPC when in the Dead state
void NPCDeadHandleEvent(GameObject *obj, Event event)
{
    NPC *npc = (NPC *)obj;
    printf("\n%s Dead HandleEvent\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);

    switch (event)
    {
    case EVENT_NONE:
        // Transition back to Idle if no specific event is triggered (e.g., after dying, NPC should stay dead until respawned)
        // You may want to handle respawning logic in the future here.
        ChangeState(obj, STATE_IDLE);
        break;
    case EVENT_RESPAWN:
        // Transition to Idle or another state (e.g., Spawn) upon respawn event
        ChangeState(obj, STATE_IDLE); // or STATE_SPAWNING if you have that state
        break;
    // Ignore Events for other cases (e.g., move, defend) as dead NPCs cannot perform these actions.
    case EVENT_DIE:
    case EVENT_ATTACK:
    case EVENT_MOVE:
    case EVENT_DEFEND:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
        case EVENT_MOVE_UP:
            break;
        case EVENT_MOVE_UP_RIGHT:
            break;
        case EVENT_MOVE_UP_LEFT:
            break;
        case EVENT_MOVE_DOWN:
            break;
        case EVENT_MOVE_DOWN_RIGHT:
            break;
        case EVENT_MOVE_DOWN_LEFT:
            break;
        case EVENT_MOVE_LEFT:
            break;
        case EVENT_MOVE_RIGHT:
            break;
        case EVENT_SHIELD:
            break;
    }
}

// Enter function for Idle state, executed once upon entering Idle
void NPCEnterIdle(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> ENTER -> Idle\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Initialization code for entering Idle state, such as resetting timers or animation.

    if (npc->base.previousState != npc->base.currentState && npc->base.currentState == STATE_IDLE)
    {
        // Setup Idle Animations
        Rectangle idle[7] = {
            {0, 128, 64, 64},   // Frame 1: Row 3, Column 1
            {64, 128, 64, 64},  // Frame 2: Row 3, Column 2
            {128, 128, 64, 64}, // Frame 3: Row 3, Column 3
            {192, 128, 64, 64}, // Frame 4: Row 3, Column 4
            {256, 128, 64, 64}, // Frame 5: Row 3, Column 5
            {320, 128, 64, 64}, // Frame 6: Row 3, Column 6
            {384, 128, 64, 64}  // Frame 7: Row 3, Column 7
        };

        // Initialize the idle animation frames and play it
        InitGameObjectAnimation(&npc->base, idle, 6, 0.2f);
    }
}

// Update function for Idle state, called repeatedly during game ticks while in Idle
void NPCUpdateIdle(GameObject *obj) {

//    Vector2 playerPos = getPlayerPos(  );
    // Set initial velocity if not moving
    if (obj->velocity.x == 0 && obj->velocity.y == 0) {
        obj->velocity.x = obj->speed;
        obj->velocity.y = obj->speed;
    }

    // Move NPC
    obj->position.x += obj->velocity.x;
    obj->position.y += obj->velocity.y;

    // Check for death condition
    if (obj->health <= 0) {
        ChangeState(obj, STATE_DEAD);
        return;
    }
    // Clamp velocity to maximum of 10f
    const float MAX_SPEED = 10.0f;
    const float SPEED_INCREASE = 1.1f;

    // Screen boundary checks with speed increase and clamping
    if (obj->position.x <= 0 || obj->position.x >= GetScreenWidth()) {
        obj->velocity.x *= -1;  // Reverse horizontal direction
        // Increase speed but clamp to maximum
        obj->velocity.x = obj->velocity.x * SPEED_INCREASE;
        if (obj->velocity.x > MAX_SPEED) obj->velocity.x = MAX_SPEED;
        if (obj->velocity.x < -MAX_SPEED) obj->velocity.x = -MAX_SPEED;
    }

    if (obj->position.y <= 0 || obj->position.y >= GetScreenHeight()) {
        obj->velocity.y *= -1;  // Reverse vertical direction
        // Increase speed but clamp to maximum
        obj->velocity.y = obj->velocity.y * SPEED_INCREASE;
        if (obj->velocity.y > MAX_SPEED) obj->velocity.y = MAX_SPEED;
        if (obj->velocity.y < -MAX_SPEED) obj->velocity.y = -MAX_SPEED;
    }

    // Update collider position
    obj->collider.p.x = obj->position.x;
    obj->collider.p.y = obj->position.y;


    UpdateAnimation(&obj->animation);
}






// Exit function for Idle state, executed once upon leaving Idle
void NPCExitIdle(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s <- EXIT <- Idle\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Cleanup code for leaving Idle state, if any.
}

// Enter function for Attacking state, executed once upon entering Attacking
void NPCEnterAttacking(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> ENTER -> Attacking\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Initialization code for entering Attacking state, such as setting up attack animations.
    Rectangle attacking[6] = {
        {0, 3328, 192, 192},   // Frame 1: Row 53, Column 1
        {192, 3328, 192, 192}, // Frame 2: Row 53, Column 2
        {384, 3328, 192, 192}, // Frame 3: Row 53, Column 3
        {576, 3520, 192, 192}, // Frame 4: Row 53, Column 4
        {768, 3520, 192, 192}, // Frame 5: Row 53, Column 5
        {960, 3520, 192, 192}  // Frame 6: Row 53, Column 6
    };

    // Initialize the idle animation frames and play it
    InitGameObjectAnimation(&npc->base, attacking, 6, 0.2f);
}

// Update function for Attacking state, called repeatedly during game ticks while in Attacking
void NPCUpdateAttacking(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> UPDATE -> Attacking\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // During game loop and game ticks, execute Attacking state behavior here, such as dealing damage.
    UpdateAnimation(&obj->animation);
}

// Exit function for Attacking state, executed once upon leaving Attacking
void NPCExitAttacking(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s <- EXIT <- Attacking\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Cleanup code for leaving Attacking state, such as resetting attack cooldown.
    UpdateAnimation(&obj->animation);
}

// Enter function for Shielding state, executed once upon entering Shielding
void NPCEnterShielding(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> ENTER -> Shielding\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Initialization code for entering Shielding state, such as enabling shield effects.

    Rectangle sheilding[8] = {
        {0, 384, 64, 64},   // Frame 1: Row 7, Column 1
        {64, 384, 64, 64},  // Frame 2: Row 7, Column 2
        {128, 384, 64, 64}, // Frame 3: Row 7, Column 3
        {192, 384, 64, 64}, // Frame 4: Row 7, Column 4
        {256, 384, 64, 64}, // Frame 5: Row 7, Column 5
        {320, 384, 64, 64}, // Frame 6: Row 7, Column 6
        {384, 384, 64, 64}, // Frame 7: Row 7, Column 7
        {448, 384, 64, 64}  // Frame 8: Row 7, Column 8
    };

    // Initialize attack animation
    InitGameObjectAnimation(&npc->base, sheilding, 6, 0.2f);
}

// Update function for Shielding state, called repeatedly during game ticks while in Shielding
void NPCUpdateShielding(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> UPDATE -> Shielding\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // During game loop and game ticks, execute Shielding state behavior here, such as reducing incoming damage.
    UpdateAnimation(&obj->animation);
}

// Exit function for Shielding state, executed once upon leaving Shielding
void NPCExitShielding(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> EXIT -> Shielding\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Cleanup code for leaving Shielding state, if any.
}

// Enter function for Dead state, executed once upon entering Dead
void NPCEnterDead(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> ENTER -> Dead\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Initialization code for entering Dead state, such as playing death animation or disabling further actions.
    Rectangle dead[6] = {
        {0, 1280, 64, 64},   // Frame 1: Row 21, Column 1
        {64, 1280, 64, 64},  // Frame 1: Row 21, Column 2
        {128, 1280, 64, 64}, // Frame 1: Row 21, Column 3
        {192, 1280, 64, 64}, // Frame 1: Row 21, Column 4
        {256, 1280, 64, 64}, // Frame 1: Row 21, Column 5
        {320, 1280, 64, 64}  // Frame 1: Row 21, Column 6
    };
    // Initialize dead animation
    InitGameObjectAnimation(&npc->base, dead, 6, 0.2f);
}

// Update function for Dead state, called repeatedly during game ticks while in Dead
void NPCUpdateDead(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> UPDATE -> Dead\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // During game loop and game ticks, execute Dead state behavior here, such as preventing any actions.
    // This could be a place to check if the NPC should be removed or respawned.
    UpdateAnimation(&obj->animation);
}

// Exit function for Dead state, executed once upon leaving Dead
void NPCExitDead(GameObject *obj)
{
    NPC *npc = (NPC *)obj;
    printf("%s -> EXIT -> Dead\n", obj->name);
    printf("Aggression: %d\n\n", npc->aggression);
    // Cleanup code for leaving Dead state, such as removing NPC from the active world, playing respawn animations, etc.
}

//Vector2 getPlayerPos(GameData *gameData)
//{
//    return gameData->player->base.position;
//}
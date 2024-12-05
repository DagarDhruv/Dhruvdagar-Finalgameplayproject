#include "../include/gameobjects/player.h"

// Initialize a new Player object with a given name
/**
 * InitPlayer - Initializes a new Player object with a given name.
 *
 * @name: The name of the Player being initialized.
 *
 * This function allocates memory for the Player object, initializes the GameObject
 * base structure, and sets the Player's texture, stamina and mana level, and state
 * machine. It also sets up necessary colliders and starting values for the Player.
 *
 * Return: A pointer to the initialized Player object, or NULL if memory allocation
 *         or texture loading fails.
 */
Player *InitPlayer(const char *name)
{
    // Allocate memory for the Player structure
    Player *player = (Player *)malloc(sizeof(Player));

    // Check if memory allocation failed
    if (!player)
    {
        // Print an error message to stderr and terminate the program if allocation fails
        fprintf(stderr, "Failed to allocate player\n");
        exit(1);
    }

    // Load player texture
    Texture2D playerTexture = LoadTexture("./assets/player_sprite_sheet.png");

    InitGameObject(&player->base,
                   name,                                                         // Name
                   (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, // Position
                   (Vector2){0, 0},                                              // Velocity
                   STATE_IDLE,                                                   // Initial State
                   GREEN,                                                        // Player Color
                   (c2Circle){                                                   // cute_c2 Circle Collider
                              .p = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
                              .r = 10},
                   (c2AABB){// AABB Collider for boundary checks
                            .min = {GetScreenWidth() / 2.0f - 10, GetScreenHeight() / 2.0f - 10},
                            .max = {GetScreenWidth() / 2.0f + 10, GetScreenHeight() / 2.0f + 10}},
                   playerTexture,
                   100 // Initial Health
    );

    // Player Specific Data
    player->stamina = 100.0f;
    player->mana = 100.0f;

    // Init the Player FSM
    InitPlayerFSM(&player->base);

    if (player->base.currentState == STATE_IDLE)
    {
        // Initialize the idle animation immediately
        PlayerEnterIdle(&player->base); // Trigger idle animation at initialization
    }

    return player;
}

/**
 * DeletePlayer - Frees the memory allocated for the Player and its associated GameObject.
 *
 * @obj: The GameObject (Player) to be cleaned up.
 *
 * This function performs the cleanup of the Player's specific resources and then
 * calls the `DeleteGameObject` function to handle the general cleanup.
 * It is responsible for ensuring no memory leaks related to the Player.
 */
void DeletePlayer(GameObject *obj)
{
    // Perform any player-specific cleanup here
    // Cast to Player if player-specific cleanup is required
    // Player *player = (Player *)obj;
    // Example of potential cleanup (not implemented here):
    // If the player is holding a dynamically allocated object, such as a Shield:
    // free(player->holding);
    // Perform any player-specific cleanup here
    DeleteGameObject(obj);
}

/**
 * InitPlayerFSM - Initializes the Finite State Machine (FSM) for the Player.
 *
 * @obj: The GameObject (Player) to initialize the FSM for.
 *
 * This function sets up the state machine for the Player, allocating memory for
 * state configurations, defining valid state transitions, and associating
 * state handler functions with each state.
 */
void InitPlayerFSM(GameObject *obj)
{
    obj->stateConfigs = (StateConfig *)malloc(sizeof(StateConfig) * STATE_COUNT);
    if (!obj->stateConfigs)
    {
        fprintf(stderr, "Failed to allocate state configs\n");
        exit(1);
    }

    // ---- STATE_IDLE state configuration ----
    // Define valid transitions from STATE_IDLE
    State idleValidTransitions[] = {STATE_WALKING, STATE_ATTACKING, STATE_SHIELD, STATE_DEAD};

    // Set up the state configuration for STATE_IDLE
    obj->stateConfigs[STATE_IDLE].name = "Player_Idle";
    obj->stateConfigs[STATE_IDLE].HandleEvent = PlayerIdleHandleEvent;
    obj->stateConfigs[STATE_IDLE].Entry = PlayerEnterIdle;
    obj->stateConfigs[STATE_IDLE].Update = PlayerUpdateIdle;
    obj->stateConfigs[STATE_IDLE].Exit = PlayerExitIdle;

    // Configure valid transitions for STATE_IDLE
    StateTransitions(&obj->stateConfigs[STATE_IDLE], idleValidTransitions, sizeof(idleValidTransitions) / sizeof(State));

    // ---- STATE_WALKING state configuration ----
    // Define valid transitions from STATE_WALKING
    State walkingValidTransitions[] = {STATE_IDLE, STATE_ATTACKING, STATE_DEAD};

    // Set up the state configuration for STATE_WALKING
    obj->stateConfigs[STATE_WALKING].name = "Player_Walking";
    obj->stateConfigs[STATE_WALKING].HandleEvent = PlayerWalkingHandleEvent;
    obj->stateConfigs[STATE_WALKING].Entry = PlayerEnterWalking;
    obj->stateConfigs[STATE_WALKING].Update = PlayerUpdateWalking;
    obj->stateConfigs[STATE_WALKING].Exit = PlayerExitWalking;

    // Configure valid transitions for STATE_WALKING
    StateTransitions(&obj->stateConfigs[STATE_WALKING], walkingValidTransitions, sizeof(walkingValidTransitions) / sizeof(State));

    // ---- STATE_ATTACKING state configuration ----
    // Define valid transitions from STATE_ATTACKING
    State attackValidTransitions[] = {STATE_IDLE, STATE_DEAD};

    // Set up the state configuration for STATE_ATTACKING
    obj->stateConfigs[STATE_ATTACKING].name = "Player_Attacking";
    obj->stateConfigs[STATE_ATTACKING].HandleEvent = PlayerAttackingHandleEvent;
    obj->stateConfigs[STATE_ATTACKING].Entry = PlayerEnterAttacking;
    obj->stateConfigs[STATE_ATTACKING].Update = PlayerUpdateAttacking;
    obj->stateConfigs[STATE_ATTACKING].Exit = PlayerExitAttacking;

    // Configure valid transitions for STATE_ATTACKING
    StateTransitions(&obj->stateConfigs[STATE_ATTACKING], attackValidTransitions, sizeof(attackValidTransitions) / sizeof(State));

    // ---- STATE_SHIELD state configuration ----
    // Define valid transitions from STATE_SHIELD
    State sheildingValidTransitions[] = {STATE_IDLE, STATE_DEAD};

    // Set up the state configuration for STATE_SHIELD
    obj->stateConfigs[STATE_SHIELD].name = "Player_Shielding";
    obj->stateConfigs[STATE_SHIELD].HandleEvent = PlayerShieldingHandleEvent;
    obj->stateConfigs[STATE_SHIELD].Entry = PlayerEnterShielding;
    obj->stateConfigs[STATE_SHIELD].Update = PlayerUpdateShielding;
    obj->stateConfigs[STATE_SHIELD].Exit = PlayerExitShielding;

    // Configure valid transitions for STATE_SHIELD
    StateTransitions(&obj->stateConfigs[STATE_SHIELD], sheildingValidTransitions, sizeof(sheildingValidTransitions) / sizeof(State));

    // ---- STATE_DEAD state configuration ----
    // Define valid transitions from STATE_DEAD
    State deadValidTransitions[] = {STATE_RESPAWN};

    // Set up the state configuration for STATE_DEAD
    obj->stateConfigs[STATE_DEAD].name = "Player_Dead";
    obj->stateConfigs[STATE_DEAD].HandleEvent = PlayerDieHandleEvent;
    obj->stateConfigs[STATE_DEAD].Entry = PlayerEnterDie;
    obj->stateConfigs[STATE_DEAD].Update = PlayerUpdateDie;
    obj->stateConfigs[STATE_DEAD].Exit = PlayerExitDie;

    // Configure valid transitions for STATE_DEAD
    StateTransitions(&obj->stateConfigs[STATE_DEAD], deadValidTransitions, sizeof(deadValidTransitions) / sizeof(State));

    // ---- STATE_RESPAWN state configuration ----
    // Define valid transitions from STATE_RESPAWN
    State respawnValidTransitions[] = {STATE_IDLE};

    // Set up the state configuration for STATE_RESPAWN
    obj->stateConfigs[STATE_RESPAWN].name = "Player_Respawn";
    obj->stateConfigs[STATE_RESPAWN].HandleEvent = PlayerRespawnHandleEvent;
    obj->stateConfigs[STATE_RESPAWN].Entry = PlayerEnterRespawn;
    obj->stateConfigs[STATE_RESPAWN].Update = PlayerUpdateRespawn;
    obj->stateConfigs[STATE_RESPAWN].Exit = PlayerExitRespawn;

    // Configure valid transitions for STATE_RESPAWN
    StateTransitions(&obj->stateConfigs[STATE_RESPAWN], respawnValidTransitions, sizeof(respawnValidTransitions) / sizeof(State));

// For unimplemented states, set them to empty defaults
#define EMPTY_STATE_CONFIG \
    (StateConfig){NULL, NULL, NULL, NULL, NULL, NULL, 0}
    obj->stateConfigs[STATE_COLLISION] = EMPTY_STATE_CONFIG;
}

// Handles events for the Player when in the Idle state
void PlayerIdleHandleEvent(GameObject *obj, Event event)
{
    // Player *player = (Player *)obj;
    // printf("\n%s Idle HandleEvent\n", obj->name);
    // printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

    switch (event)
    {
    case EVENT_MOVE:
        // Transition back to Idle if no specific event is triggered
        ChangeState(obj, STATE_WALKING);
        break;
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
    case EVENT_NONE:
        obj->previousState = obj->currentState;
        break;
    // Ignore Events for other cases
    case EVENT_RESPAWN:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
    }
}

// Handles events for the Player when in the Walking state
void PlayerWalkingHandleEvent(GameObject *obj, Event event)
{
    Player *player = (Player *)obj;
    printf("%s Walking HandleEvent\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

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
    case EVENT_MOVE:
    case EVENT_DEFEND:
    case EVENT_RESPAWN:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
    }
}

// Handles events for the Player when in the Attacking state
void PlayerAttackingHandleEvent(GameObject *obj, Event event)
{
    Player *player = (Player *)obj;
    printf("\n%s Attacking HandleEvent\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

    switch (event)
    {
    case EVENT_NONE:
        // Transition back to Idle if no specific event is triggered
        ChangeState(obj, STATE_IDLE);
        break;
    case EVENT_DIE:
        // Transition to Dead state if a die event is received
        ChangeState(obj, STATE_DEAD);
        break;
    // Ignore Events for other cases
    case EVENT_MOVE:
    case EVENT_ATTACK:
    case EVENT_DEFEND:
    case EVENT_RESPAWN:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
    }
}

// Handles events for the Player when in the Shielding state
void PlayerShieldingHandleEvent(GameObject *obj, Event event)
{
    Player *player = (Player *)obj;
    printf("\n%s Sheilding HandleEvent\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

    switch (event)
    {
    case EVENT_NONE:
        // Transition back to Idle if no specific event is triggered
        ChangeState(obj, STATE_IDLE);
        break;
    case EVENT_DIE:
        // Transition to Dead state if a die event is received
        ChangeState(obj, STATE_DEAD);
        break;
    // Ignore Events for other cases
    case EVENT_MOVE:
    case EVENT_ATTACK:
    case EVENT_DEFEND:
    case EVENT_RESPAWN:
    case EVENT_COLLISION_START:
    case EVENT_COLLISION_END:
    case EVENT_COUNT:
        break;
    }
}

// Handles events for the Player when in the Die state
void PlayerDieHandleEvent(GameObject *obj, Event event)
{
    Player *player = (Player *)obj;
    printf("\n%s Die HandleEvent\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    (void)event; // ignoring event
}

// Handles events for the Player when in the Respawn state
void PlayerRespawnHandleEvent(GameObject *obj, Event event)
{
    Player *player = (Player *)obj;
    printf("\n%s Die HandleEvent\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    (void)event; // ignoring event
}

// Common movement function to handle state and animation transitions
void PlayerMove(Player *player, Vector2 moveDirection)
{
    player->base.position.x += moveDirection.x;
    player->base.position.y += moveDirection.y;

    // Update Collider
    player->base.collider.p.x = player->base.position.x;
    player->base.collider.p.y = player->base.position.y;
}

// Select a Random Idle Animation
void SelectRandomIdleAnimation(GameObject *obj)
{

    // See grid_player_sprite_sheet.png for rows and columns
    int randomChoice = rand() % 7 + 1;

    Rectangle idle1[8] = {
        {0, 320, 64, 64},   // Frame 1: Row 6, Column 1
        {64, 320, 64, 64},  // Frame 2: Row 6, Column 2
        {128, 320, 64, 64}, // Frame 3: Row 6, Column 3
        {192, 320, 64, 64}, // Frame 4: Row 6, Column 4
        {256, 320, 64, 64}, // Frame 5: Row 6, Column 5
        {320, 320, 64, 64}, // Frame 6: Row 6, Column 6
        {384, 320, 64, 64}, // Frame 7: Row 6, Column 7
        {448, 320, 64, 64}  // Frame 8: Row 6, Column 8
    };

    Rectangle idle2[8] = {
        {0, 384, 64, 64},   // Frame 1: Row 7, Column 1
        {64, 384, 64, 64},  // Frame 2: Row 7, Column 2
        {128, 384, 64, 64}, // Frame 3: Row 7, Column 3
        {192, 384, 64, 64}, // Frame 4: Row 7, Column 4
        {256, 384, 64, 64}, // Frame 5: Row 7, Column 5
        {320, 384, 64, 64}, // Frame 6: Row 7, Column 6
        {384, 384, 64, 64}, // Frame 7: Row 7, Column 7
        {448, 384, 64, 64}  // Frame 8: Row 7, Column 8
    };

    Rectangle idle3[8] = {
        {0, 448, 64, 64},   // Frame 1: Row 8, Column 1
        {64, 448, 64, 64},  // Frame 2: Row 8, Column 2
        {128, 448, 64, 64}, // Frame 3: Row 8, Column 3
        {192, 448, 64, 64}, // Frame 4: Row 8, Column 4
        {256, 448, 64, 64}, // Frame 5: Row 8, Column 5
        {320, 448, 64, 64}, // Frame 6: Row 8, Column 6
        {384, 448, 64, 64}, // Frame 7: Row 8, Column 7
        {448, 448, 64, 64}  // Frame 8: Row 8, Column 8
    };

    Rectangle idle4[13] = {
        {0, 1024, 64, 64},   // Frame 1: Row 17, Column 1
        {64, 1024, 64, 64},  // Frame 2: Row 17, Column 2
        {128, 1024, 64, 64}, // Frame 3: Row 17, Column 3
        {192, 1024, 64, 64}, // Frame 4: Row 17, Column 4
        {256, 1024, 64, 64}, // Frame 5: Row 17, Column 5
        {320, 1024, 64, 64}, // Frame 6: Row 17, Column 6
        {384, 1024, 64, 64}, // Frame 7: Row 17, Column 7
        {448, 1024, 64, 64}, // Frame 8: Row 17, Column 8
        {512, 1024, 64, 64}, // Frame 9: Row 17, Column 9
        {576, 1024, 64, 64}, // Frame 10: Row 17, Column 10
        {640, 1024, 64, 64}, // Frame 11: Row 17, Column 11
        {704, 1024, 64, 64}, // Frame 12: Row 17, Column 12
        {768, 1024, 64, 64}  // Frame 13: Row 17, Column 13
    };

    Rectangle idle5[13] = {
        {0, 1088, 64, 64},   // Frame 1: Row 18, Column 1
        {64, 1088, 64, 64},  // Frame 2: Row 18, Column 2
        {128, 1088, 64, 64}, // Frame 3: Row 18, Column 3
        {192, 1088, 64, 64}, // Frame 4: Row 18, Column 4
        {256, 1088, 64, 64}, // Frame 5: Row 18, Column 5
        {320, 1088, 64, 64}, // Frame 6: Row 18, Column 6
        {384, 1088, 64, 64}, // Frame 7: Row 18, Column 7
        {448, 1088, 64, 64}, // Frame 8: Row 18, Column 8
        {512, 1088, 64, 64}, // Frame 9: Row 18, Column 9
        {576, 1088, 64, 64}, // Frame 10: Row 18, Column 10
        {640, 1088, 64, 64}, // Frame 11: Row 18, Column 11
        {704, 1088, 64, 64}, // Frame 12: Row 18, Column 12
        {768, 1088, 64, 64}  // Frame 13: Row 18, Column 13
    };

    Rectangle idle6[13] = {
        {0, 1152, 64, 64},   // Frame 1: Row 19, Column 1
        {64, 1152, 64, 64},  // Frame 2: Row 19, Column 2
        {128, 1152, 64, 64}, // Frame 3: Row 19, Column 3
        {192, 1152, 64, 64}, // Frame 4: Row 19, Column 4
        {256, 1152, 64, 64}, // Frame 5: Row 19, Column 5
        {320, 1152, 64, 64}, // Frame 6: Row 19, Column 6
        {384, 1152, 64, 64}, // Frame 7: Row 19, Column 7
        {448, 1152, 64, 64}, // Frame 8: Row 19, Column 8
        {512, 1152, 64, 64}, // Frame 9: Row 19, Column 9
        {576, 1152, 64, 64}, // Frame 10: Row 19, Column 10
        {640, 1152, 64, 64}, // Frame 11: Row 19, Column 11
        {704, 1152, 64, 64}, // Frame 12: Row 19, Column 12
        {768, 1152, 64, 64}  // Frame 13: Row 19, Column 13
    };

    Rectangle idle7[13] = {
        {0, 1216, 64, 64},   // Frame 1: Row 20, Column 1
        {64, 1216, 64, 64},  // Frame 2: Row 20, Column 2
        {128, 1216, 64, 64}, // Frame 3: Row 20, Column 3
        {192, 1216, 64, 64}, // Frame 4: Row 20, Column 4
        {256, 1216, 64, 64}, // Frame 5: Row 20, Column 5
        {320, 1216, 64, 64}, // Frame 6: Row 20, Column 6
        {384, 1216, 64, 64}, // Frame 7: Row 20, Column 7
        {448, 1216, 64, 64}, // Frame 8: Row 20, Column 8
        {512, 1216, 64, 64}, // Frame 9: Row 20, Column 9
        {576, 1216, 64, 64}, // Frame 10: Row 20, Column 10
        {640, 1216, 64, 64}, // Frame 11: Row 20, Column 11
        {704, 1216, 64, 64}, // Frame 12: Row 20, Column 12
        {768, 1216, 64, 64}  // Frame 13: Row 20, Column 13
    };

    switch (randomChoice)
    {
    case 1:
        InitGameObjectAnimation(obj, idle1, 8, 0.2f); // Animation 1
        break;
    case 2:
        InitGameObjectAnimation(obj, idle2, 8, 0.2f); // Animation 2
        break;
    case 3:
        InitGameObjectAnimation(obj, idle3, 8, 0.2f); // Animation 3
        break;
    case 4:
        InitGameObjectAnimation(obj, idle4, 8, 0.2f); // Animation 3
        break;
    case 5:
        InitGameObjectAnimation(obj, idle5, 8, 0.2f); // Animation 3
        break;
    case 6:
        InitGameObjectAnimation(obj, idle6, 8, 0.2f); // Animation 3
        break;
    case 7:
        InitGameObjectAnimation(obj, idle7, 8, 0.2f); // Animation 3
        break;
    default:
        InitGameObjectAnimation(obj, idle1, 8, 0.2f); // Default to Animation 1
        break;
    }
}

void PlayerEnterIdle(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> ENTER -> Idle\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

    if (player->base.previousState != player->base.currentState && player->base.currentState == STATE_IDLE)
    {
        SelectRandomIdleAnimation(&player->base);
    }
}

void PlayerUpdateIdle(GameObject *obj)
{
    // Player *player = (Player *)obj;
    // printf("\n%s -> UPDATE -> Idle\n", obj->name);
    // printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    //  Complete the remainder of the method
    UpdateAnimation(&obj->animation);

    // Check if the animation has finished
    if (obj->animation.currentFrame == obj->animation.frameCount - 1)
    {
        // Transition to a another idle animation
        SelectRandomIdleAnimation(obj); // Trigger idle animation
    }
}

void PlayerExitIdle(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s <- EXIT <- Idle\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
}

void PlayerEnterWalking(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> ENTER -> Walking\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method

    // Moving Up Frames Default for moving
    Rectangle walk[9] = {
        {0, 512, 64, 64},   // Frame 1: Row 8, Column 1
        {64, 512, 64, 64},  // Frame 2: Row 8, Column 2
        {128, 512, 64, 64}, // Frame 3: Row 8, Column 3
        {192, 512, 64, 64}, // Frame 4: Row 8, Column 4
        {256, 512, 64, 64}, // Frame 5: Row 8, Column 5
        {320, 512, 64, 64}, // Frame 6: Row 8, Column 6
        {384, 512, 64, 64}, // Frame 7: Row 8, Column 7
        {448, 512, 64, 64}, // Frame 8: Row 8, Column 8
        {512, 512, 64, 64}  // Frame 9: Row 8, Column 9
    };

    InitGameObjectAnimation(&player->base, walk, 9, 0.1f);

    // Moving Down
    // Place this in its own method
    // Rectangle frames[9] = {
    //     {0, 640, 64, 64},   // Frame 0
    //     {64, 640, 64, 64},  // Frame 1
    //     {128, 640, 64, 64}, // Frame 2
    //     {192, 640, 64, 64}, // Frame 3
    //     {256, 640, 64, 64}, // Frame 4
    //     {320, 640, 64, 64}, // Frame 5
    //     {384, 640, 64, 64}, // Frame 6
    //     {448, 640, 64, 64}, // Frame 7
    //     {512, 640, 64, 64}  // Frame 8
    // };

    // PlayerMove(player, (Vector2){0, 1}, BLUE, frames, 9);

    // Moving Left
    // Place this in its own method
    // Rectangle frames[9] = {
    //     {0, 576, 64, 64},   // Frame 0
    //     {64, 576, 64, 64},  // Frame 1
    //     {128, 576, 64, 64}, // Frame 2
    //     {192, 576, 64, 64}, // Frame 3
    //     {256, 576, 64, 64}, // Frame 4
    //     {320, 576, 64, 64}, // Frame 5
    //     {384, 576, 64, 64}, // Frame 6
    //     {448, 576, 64, 64}, // Frame 7
    //     {512, 576, 64, 64}  // Frame 8
    // };

    // PlayerMove(player, (Vector2){-1, 0}, BLUE, frames, 9);

    // Moving Right
    // Place this in its own method
    // Rectangle frames[9] = {
    //     {0, 704, 64, 64},   // Frame 0
    //     {64, 704, 64, 64},  // Frame 1
    //     {128, 704, 64, 64}, // Frame 2
    //     {192, 704, 64, 64}, // Frame 3
    //     {256, 704, 64, 64}, // Frame 4
    //     {320, 704, 64, 64}, // Frame 5
    //     {384, 704, 64, 64}, // Frame 6
    //     {448, 704, 64, 64}, // Frame 7
    //     {512, 704, 64, 64}  // Frame 8
    // };
    // PlayerMove(player, (Vector2){1, 0}, BLUE, frames, 9);
}

void PlayerUpdateWalking(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> UPDATE -> Walking\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    PlayerMove(player, (Vector2){0, -1});
    UpdateAnimation(&obj->animation);
}

void PlayerExitWalking(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s <- EXIT <- Walking\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
}

void PlayerEnterAttacking(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> ENTER -> Attacking\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    // Example: Deduct some stamina when attacking

    // Attack animation (or other actions as needed)
    Rectangle attacking[6] = {
        {0, 2952, 192, 192},   // Frame 1: Row 44, Column 1
        {192, 2952, 192, 192}, // Frame 2: Row 44, Column 2
        {384, 2952, 192, 192}, // Frame 3: Row 44, Column 3
        {576, 2952, 192, 192}, // Frame 4: Row 44, Column 4
        {768, 2952, 192, 192}, // Frame 5: Row 44, Column 5
        {960, 2952, 192, 192}  // Frame 6: Row 44, Column 6
    };

    InitGameObjectAnimation(&player->base, attacking, 6, 0.1f);
}

void PlayerUpdateAttacking(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> UPDATE -> Attacking\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    // Check if the attack should end or be interrupted (e.g., stamina depletion)
    UpdateAnimation(&obj->animation);
}

void PlayerExitAttacking(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s <- EXIT <- Attacking\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    // Reset or adjust any temporary changes during attack, if needed
}

void PlayerEnterShielding(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> ENTER -> Sheilding\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    // Example: Deduct some stamina for shielding
}
void PlayerUpdateShielding(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s -> UPDATE -> Sheilding\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    // Example: Check if the shielding duration is over or if stamina is depleted
    UpdateAnimation(&obj->animation);
}

void PlayerExitShielding(GameObject *obj)
{
    Player *player = (Player *)obj;
    printf("\n%s <- EXIT <- Sheilding\n", obj->name);
    printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
    // Complete the remainder of the method
    // Reset any temporary shielding effects if necessary
}

void PlayerEnterDie(GameObject *obj)
{
    printf("\n%s -> ENTER -> Die\n", obj->name);
    // Complete the remainder of the method
}

void PlayerUpdateDie(GameObject *obj)
{
    printf("\n%s -> UPDATE -> Die\n", obj->name);
    ChangeState(obj, STATE_RESPAWN);
    // Complete the remainder of the method
    UpdateAnimation(&obj->animation);
}

void PlayerExitDie(GameObject *obj)
{
    printf("\n%s <- EXIT <- Die\n", obj->name);
    // Complete the remainder of the method
}

void PlayerEnterRespawn(GameObject *obj)
{
    printf("\n%s -> ENTER -> Respawn\n", obj->name);
    // Complete the remainder of the method
}

void PlayerUpdateRespawn(GameObject *obj)
{
    printf("\n%s -> UPDATE -> Respawn\n", obj->name);
    ChangeState(obj, STATE_IDLE);
    // Complete the remainder of the method
    UpdateAnimation(&obj->animation);
}

void PlayerExitRespawn(GameObject *obj)
{
    printf("\n%s <- EXIT <- Respawn\n", obj->name);
    // Complete the remainder of the method
}
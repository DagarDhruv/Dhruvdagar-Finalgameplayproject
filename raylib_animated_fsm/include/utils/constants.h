#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Buffer zone to avoid stuck states in collision detection
static const float COLLISION_BUFFER = 2.0f;
static const float COLLISION_PUSH_BACK = 2.0f;

// Firing Cooldown (0.1 seconds)
static const double COMMAND_FIRE_COOLDOWN = 0.1f;

// Firing Trigger Treshold
static const float FIRING_TRIGGER_TRESHOLD = 0.1f;

// Tumbstick deadzone
static const float TUMBSTICK_DEADZONE_THRESHOLD = 0.2f;

// Thresholds for directional movement
static const float MOVE_VERTICAL_THRESHOLD = 0.5f;
static const float MOVE_HORIZONTAL_THRESHOLD = 0.5f;
static const float MOVE_DIAGONAL_THRESHOLD = 0.5f;

#endif // CONSTANTS_H
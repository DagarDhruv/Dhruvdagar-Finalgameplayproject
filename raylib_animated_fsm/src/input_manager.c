#include <stdio.h>
#include <math.h>

#include <raylib.h>

#include "../include/utils/input_manager.h"
#include "../include/utils/constants.h"

/**
 * InitInputManager - Initialises input management settings.
 *
 * This function is responsible for initialising any required input sources,
 * such as the keyboard and game controller. It may also include any setup
 * for handling multiple input devices in a game.
 *
 * Currently, it does not contain any implementation, but it provides a place
 * to set up any input-related configurations if needed in the future.
 */
void InitInputManager()
{
    // Initialize input sources (keyboard, controller, etc.)
}

/**
 * PollInput - Captures and interprets player input from a gamepad or keyboard.
 *
 * This function first checks for input from a gamepad. If a gamepad is detected
 * and active, it handles inputs from the D-pad, thumbsticks, and trigger buttons.
 * If no gamepad input is detected, it checks the keyboard for key presses and
 * returns corresponding commands.
 *
 * Returns the appropriate command (from an enumerated Command type) based on
 * detected input, or COMMAND_NONE if no relevant input is detected.
 */
Command PollInput()
{
    // Check for gamepad input first
    if (IsGamepadAvailable(0))
    {
        // Get values for left thumbstick and right trigger
        float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
        float rightTrigger = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER);

        // Determine if any gamepad input exceeds threshold levels
        bool gamepadActive = (fabs(leftStickX) > TUMBSTICK_DEADZONE_THRESHOLD ||
                              fabs(leftStickY) > TUMBSTICK_DEADZONE_THRESHOLD ||
                              rightTrigger > FIRING_TRIGGER_TRESHOLD ||
                              IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
                              IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) ||
                              IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
                              IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));

        // If the gamepad is active, determine specific command based on input
        if (gamepadActive)
        {
            // Check D-pad directional buttons for movement commands
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
                return COMMAND_MOVE_UP;
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
                return COMMAND_MOVE_DOWN;
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
                return COMMAND_MOVE_LEFT;
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
                return COMMAND_MOVE_RIGHT;

            // Check thumbstick for directional input, prioritising vertical movement
            if (fabs(leftStickY) > fabs(leftStickX))
            {
                if (leftStickY < -MOVE_VERTICAL_THRESHOLD)
                    return COMMAND_MOVE_UP;
                if (leftStickY > MOVE_VERTICAL_THRESHOLD)
                    return COMMAND_MOVE_DOWN;
            }
            else
            {
                if (leftStickX < -MOVE_HORIZONTAL_THRESHOLD)
                    return COMMAND_MOVE_LEFT;
                if (leftStickX > MOVE_HORIZONTAL_THRESHOLD)
                    return COMMAND_MOVE_RIGHT;
            }

            if (leftStickX > MOVE_HORIZONTAL_THRESHOLD && leftStickY < -MOVE_VERTICAL_THRESHOLD)
                return COMMAND_MOVE_UP_RIGHT;
            if (leftStickX < -MOVE_HORIZONTAL_THRESHOLD && leftStickY < -MOVE_VERTICAL_THRESHOLD)
                return COMMAND_MOVE_UP_LEFT;
            if (leftStickX > MOVE_HORIZONTAL_THRESHOLD && leftStickY > MOVE_VERTICAL_THRESHOLD)
                return COMMAND_MOVE_DOWN_RIGHT;
            if (leftStickX < -MOVE_HORIZONTAL_THRESHOLD && leftStickY > MOVE_VERTICAL_THRESHOLD)
                return COMMAND_MOVE_DOWN_LEFT;

            // Check right trigger for firing command
            if (rightTrigger > FIRING_TRIGGER_TRESHOLD)
                return COMMAND_ATTACK;

            // No specific gamepad input detected, return no command
            return COMMAND_NONE;
        }
    }

    // If no gamepad input, check keyboard for movement and action commands

    if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W))
    {
        if (IsKeyDown(KEY_A)) 
            return COMMAND_MOVE_UP_LEFT;    // W + A = Up-Left
        if (IsKeyDown(KEY_D)) 
            return COMMAND_MOVE_UP_RIGHT;   // W + D = Up-Right
        return COMMAND_MOVE_UP;
    }
    if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S))
    {
        if (IsKeyDown(KEY_A)) 
            return COMMAND_MOVE_DOWN_LEFT;  // S + A = Down-Left
        if (IsKeyDown(KEY_D)) 
            return COMMAND_MOVE_DOWN_RIGHT; // S + D = Down-Right
        return COMMAND_MOVE_DOWN;
    }
    
    if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W))
        return COMMAND_MOVE_UP;
    if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S))
        return COMMAND_MOVE_DOWN;
    if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A))
        return COMMAND_MOVE_LEFT;
    if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D))
        return COMMAND_MOVE_RIGHT;
    if (IsKeyPressed(KEY_SPACE) || IsKeyDown(KEY_SPACE))
        return COMMAND_ATTACK;
    if (IsKeyPressed(KEY_I))
        return COMMAND_COLLISION_START;
    if (IsKeyPressed(KEY_O))
        return COMMAND_COLLISION_END;
    if (IsKeyPressed(KEY_M))
        return COMMAND_SHIELD;


    // No input detected, return no command
    return COMMAND_NONE;
}

/**
 * ExitInputManager - Cleans up input management resources if required.
 *
 * This function can be used to release or reset any resources related to input
 * management, such as disconnecting input devices or clearing configurations.
 * Currently, it does not contain any implementation but serves as a placeholder
 * for future resource management if needed.
 */
void ExitInputManager()
{
    // Cleanup resources if needed
}
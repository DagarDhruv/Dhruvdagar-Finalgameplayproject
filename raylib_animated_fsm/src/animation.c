#include <stdlib.h>
#include "../include/animation/animation.h"

/**
 * InitAnimation - Initialises an animation with the given parameters.
 *
 * @animationData: A pointer to the AnimationData structure that will store
 *                 all animation-related information, including frames and timing.
 * @texture:       The Texture2D object containing the sprite sheet or animation
 *                 frames to be used for rendering.
 * @frames:        An array of Rectangle objects representing each animation frame
 *                 on the texture.
 * @frameCount:    The total number of frames in the animation.
 * @frameDuration: The duration each frame should be displayed, in seconds.
 * @loop:          A boolean indicating whether the animation should loop
 *                 back to the first frame after the last frame.
 *
 * This function allocates memory for the frames array in animationData and copies
 * each frame from the provided frames array. It also sets initial values for other
 * animation properties, such as frame count, duration, loop setting, and starting
 * with the animation active and set to the first frame.
 */
void InitAnimation(AnimationData *animationData,
                   Texture2D texture,
                   Rectangle *frames,
                   int frameCount,
                   float frameDuration,
                   bool loop)
{
    // Store texture and allocate memory for frame data in animationData
    animationData->texture = texture;
    animationData->frames = (Rectangle *)malloc(frameCount * sizeof(Rectangle));

    // Copy each frame from frames array to animationData's frames
    if (animationData->frames != NULL)
    {
        for (int i = 0; i < frameCount; i++)
        {
            animationData->frames[i] = frames[i];
        }
    }

    // Initialise animation properties
    animationData->frameCount = frameCount;
    animationData->frameDuration = frameDuration;
    animationData->loop = loop;
    animationData->currentFrame = 0;  // Start at the first frame
    animationData->frameTimer = 0.0f; // Reset frame timer to zero
    animationData->active = true;     // Set animation as active by default
}

/**
 * UpdateAnimation - Updates the animation's current frame based on time elapsed.
 *
 * @animationData: A pointer to the AnimationData structure containing the
 *                 animation's current state and properties.
 *
 * This function increments the frame timer by the time elapsed since the last frame.
 * If the frame timer exceeds the specified frame duration, it advances to the next frame.
 * If the animation has reached the last frame, it either loops back to the start
 * (if looping is enabled) or holds on the last frame (if looping is disabled).
 */
void UpdateAnimation(AnimationData *animationData)
{
    // If the animation is inactive, return immediately
    if (!animationData->active)
    {
        return;
    }

    // Update frame timer with delta time (time elapsed since last frame)
    animationData->frameTimer += GetFrameTime();

    // Check if it's time to advance to the next frame
    if (animationData->frameTimer >= animationData->frameDuration)
    {
        animationData->currentFrame++; // Move to the next frame

        // Check if the end of the animation frames is reached
        if (animationData->currentFrame >= animationData->frameCount)
        {
            if (animationData->loop)
            {
                animationData->currentFrame = 0; // Restart at first frame if looping
            }
            else
            {
                animationData->currentFrame = animationData->frameCount - 1; // Stay on last frame if not looping
            }
        }

        // Reset the frame timer after advancing frames
        animationData->frameTimer = 0.0f;
    }
}

/**
 * RenderAnimation - Renders the current frame of the animation at a specified position.
 *
 * @animationData: A constant pointer to the AnimationData structure, which holds
 *                 all the information needed to display the animation.
 * @position:      A Vector2 specifying the x and y screen coordinates where the
 *                 animation should be drawn.
 * @tint:          A Color value to apply as a tint over the animation's texture.
 *
 * This function retrieves the current frame based on the currentFrame index in
 * animationData, adjusts the drawing position to center the texture, and renders
 * the frame at the specified position with the given tint.
 */
void RenderAnimation(const AnimationData *animationData, Vector2 position, Color tint)
{
    // If the animation is inactive, don't render it
    if (!animationData->active)
    {
        return;
    }

    // Retrieve the rectangle for the current frame
    Rectangle frame = animationData->frames[animationData->currentFrame];

    // Adjust the drawing position so the animation is centered at the specified point
    Vector2 adjustedPosition = {
        position.x - frame.width / 2, // Offset X by half the frame width
        position.y - frame.height / 2 // Offset Y by half the frame height
    };

    // Render the current frame with the given tint color
    DrawTextureRec(
        animationData->texture,
        frame,
        adjustedPosition,
        tint);
}

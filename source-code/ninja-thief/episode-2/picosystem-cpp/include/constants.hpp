#pragma once

#include <cstdint>

namespace Constants {
    // Screen size in pixels
    const uint8_t SCREEN_WIDTH = 120;
    const uint8_t SCREEN_HEIGHT = 120;

    // Actual game area in pixels
    const uint8_t GAME_WIDTH = 120;
    const uint8_t GAME_HEIGHT = 120;

    // Each sprite on the spritesheet is 8x8 pixels
    const uint8_t SPRITE_SIZE = 8;

    // Size of the spritesheet in pixels (only needed for PicoSystem)
    const uint8_t SPRITESHEET_WIDTH = SPRITE_SIZE * 8;
    const uint8_t SPRITESHEET_HEIGHT = SPRITE_SIZE * 6;

    // Offset of game area from top left corner
    const float GAME_OFFSET_X = (SCREEN_WIDTH - GAME_WIDTH) / 2;
    const float GAME_OFFSET_Y = (SCREEN_HEIGHT - GAME_HEIGHT) / 2;

    // Sprite data, including indices to use for rendering
    namespace Sprites {
        // Offset of the red ninja sprites from the blue ninja sprites
        const uint8_t RED_OFFSET = 4;

        // We'll only be using PLAYER_IDLE for now
        const uint8_t PLAYER_IDLE = 32;
        const uint8_t PLAYER_CLIMBING_IDLE = 33;
        const uint8_t PLAYER_CLIMBING_1 = 34;
        const uint8_t PLAYER_CLIMBING_2 = 35;
        const uint8_t PLAYER_WALKING_1 = 40;
        const uint8_t PLAYER_WALKING_2 = 41;
        const uint8_t PLAYER_JUMPING_UP = 42;
        const uint8_t PLAYER_JUMPING_DOWN = 43;

        const uint8_t LADDER = 11;

        const uint8_t COIN = 19;
        const uint8_t GEM = 18;
    }

    // Player data such as speeds
    namespace Player {
        // Speeds are measured in pixels per second
        const float MAX_SPEED = 50.0f;

        const float JUMP_SPEED = 125.0f;
    }

    // Environment data such as gravity strength
    namespace Environment {
        const float GRAVITY_ACCELERATION = 375.0f;
    }
}
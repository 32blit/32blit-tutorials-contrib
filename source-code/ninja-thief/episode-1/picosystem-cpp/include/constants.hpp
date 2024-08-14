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
}
#include "ninja.hpp"

using namespace picosystem;

Ninja::Ninja() {

}

Ninja::Ninja(float x, float y) : position_x(x), position_y(y) {

}

void Ninja::update(float dt, Constants::LevelData& level_data) {
    // Apply gravity
    velocity_y += Constants::Environment::GRAVITY_ACCELERATION * dt;

    // Move the ninja
    position_x += velocity_x * dt;
    position_y += velocity_y * dt;

    // Don't allow ninja to go off the sides
    if (position_x < -Constants::Ninja::BORDER) {
        position_x = -Constants::Ninja::BORDER;
    }
    else if (position_x > Constants::GAME_WIDTH - Constants::Ninja::BORDER - Constants::Ninja::WIDTH) {
        position_x = Constants::GAME_WIDTH - Constants::Ninja::BORDER - Constants::Ninja::WIDTH;
    }

    // Detect and resolve any collisions with platforms, ladders, coins etc
    handle_collisions(level_data);

    // Update direction the ninja is facing (only if the ninja is moving)
    if (velocity_x < 0.0f) {
        facing_direction = HorizontalDirection::LEFT;
    }
    else if (velocity_x > 0.0f) {
        facing_direction = HorizontalDirection::RIGHT;
    }
}

void Ninja::render() {
    // If ninja is travelling left, flip the image horizontally (set the transform flags)
    uint32_t transform_flags = facing_direction == HorizontalDirection::RIGHT ? 0 : HFLIP;

    // Since we are specifying the transform flags, we need to include all the extra parameters for sprite():
    //  - number of sprites across (1)
    //  - number of sprites down (1)
    //  - width to stretch to (SPRITE_SIZE - i.e. don't stretch)
    //  - height to stretch to (SPRITE_SIZE - i.e. don't stretch)
    sprite(Constants::Sprites::PLAYER_IDLE, std::round(position_x) + Constants::GAME_OFFSET_X, std::round(position_y) + Constants::GAME_OFFSET_Y, 1, 1, Constants::SPRITE_SIZE, Constants::SPRITE_SIZE, transform_flags);
}

bool Ninja::check_colliding(float object_x, float object_y, uint8_t object_size) {
    return (position_x + Constants::SPRITE_SIZE - Constants::Ninja::BORDER > object_x &&
        position_x + Constants::Ninja::BORDER < object_x + object_size &&
        position_y + Constants::SPRITE_SIZE > object_y &&
        position_y < object_y + object_size);
}

void Ninja::handle_collisions(Constants::LevelData& level_data) {
    // Get position of ninja in "grid" of tiles
    // We're relying on converting to integers to truncate and hence round down
    uint8_t x = position_x / Constants::SPRITE_SIZE;
    uint8_t y = position_y / Constants::SPRITE_SIZE;

    // Check the four tiles which the ninja might be colliding with (the top left tile is marked by the x and y previously calculated)

    // We need to check that the player is within the game area
    // If they aren't, we don't need to worry about checking for collisions
    if (x < Constants::GAME_WIDTH_TILES && y < Constants::GAME_HEIGHT_TILES && position_x >= -Constants::Ninja::BORDER && position_y >= -Constants::SPRITE_SIZE) {

        // It's possible the ninja is near the edge of the screen and we could end up checking tiles which don't exist (off the edge of the screen)
        // To avoid this issue, we use the ternary operator to vary the maximum x and y offsets
        // The minimum offset is handled by the trucation, since it will round up (rather than down) if the value is negative
        for (uint8_t y_offset = 0; y_offset < (y == Constants::GAME_HEIGHT_TILES - 1 ? 1 : 2); y_offset++) {

            for (uint8_t x_offset = 0; x_offset < (x == Constants::GAME_WIDTH_TILES - 1 ? 1 : 2); x_offset++) {
				
				// Calculate grid position of this tile
				uint8_t new_x = x + x_offset;
				uint8_t new_y = y + y_offset;

				// Handle platforms
				handle_platform(level_data, new_x, new_y);
            }
        }
    }
}

void Ninja::handle_platform(Constants::LevelData& level_data, uint8_t x, uint8_t y) {
    // Get tile's sprite index from level data
    uint8_t tile_id = level_data.platforms[y * Constants::GAME_WIDTH_TILES + x];

    // Check the tile actually exists (check that it isn't blank)
    if (tile_id != Constants::Sprites::BLANK_TILE) {

        // Calculate the actual position of the tile from the grid position
        float tile_x = x * Constants::SPRITE_SIZE;
        float tile_y = y * Constants::SPRITE_SIZE;

        // Check if the ninja is colliding with the tile
        if (check_colliding(tile_x, tile_y, Constants::SPRITE_SIZE)) {

            // Resolve collision by finding the direction with the least intersection
            // The value of the direction variable corresponds to:
            // 0 - left side of tile
            // 1 - top side of tile
            // 2 - right side of tile
            // 3 - bottom side of tile
            uint8_t direction = 0;

            // The starting value of least_intersection is at least the maximum possible intersection
            // The width/height of the tile is the maximum intersection possible
            float least_intersection = Constants::SPRITE_SIZE;

            // Check each side of the tile and find the minimum intersection

            // Left side of tile
            float intersection = position_x + Constants::Ninja::WIDTH + Constants::Ninja::BORDER - tile_x;
            if (intersection < least_intersection) {
                direction = 0;
                least_intersection = intersection;
            }

            // Top side of tile
            intersection = position_y + Constants::SPRITE_SIZE - tile_y;
            if (intersection < least_intersection) {
                direction = 1;
                least_intersection = intersection;
            }

            // Right side of tile
            intersection = tile_x + Constants::SPRITE_SIZE - position_x - Constants::Ninja::BORDER;
            if (intersection < least_intersection) {
                direction = 2;
                least_intersection = intersection;
            }

            // Bottom side of tile
            intersection = tile_y + Constants::SPRITE_SIZE - position_y;
            if (intersection < least_intersection) {
                direction = 3;
                least_intersection = intersection;
            }

            // Now resolve collision by moving the ninja in the direction of least intersection, by exactly the amount equal to the least intersection
            switch (direction) {
            case 0:
                // Hit the left side of a platform
                position_x -= least_intersection;
                velocity_x = 0.0f;
                break;

            case 1:
                // Landed on top of a platform
                position_y -= least_intersection;
                velocity_y = 0.0f;
                break;

            case 2:
                // Hit the right side of a platform
                position_x += least_intersection;
                velocity_x = 0.0f;
                break;

            case 3:
                // Hit the underside of a platform
                position_y += least_intersection;
                velocity_y = 0.0f;
                break;

            default:
                break;
            }
        }
    }
}

void Ninja::jump(float jump_speed) {
    // Upwards is negative
    velocity_y = -jump_speed;
}
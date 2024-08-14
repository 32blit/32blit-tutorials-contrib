#include "level.hpp"

using namespace picosystem;

Level::Level() {

}

Level::Level(uint8_t _level_number) {
    level_number = _level_number;
    level_data = Constants::LEVELS[level_number];

    // Temporary player object for testing
    player = PlayerNinja(50, 50);
}

void Level::update(float dt) {
    // Update player
    player.update(dt, level_data);
}

void Level::render() {
    // Render platform tiles
    for (uint8_t y = 0; y < Constants::GAME_HEIGHT_TILES; y++) {
        for (uint8_t x = 0; x < Constants::GAME_WIDTH_TILES; x++) {

            // Calculate tile index
            uint8_t tile_id = level_data.platforms[y * Constants::GAME_WIDTH_TILES + x];

            // Only render the tile if it isn't a blank tile
            if (tile_id != Constants::Sprites::BLANK_TILE) {
                sprite(tile_id, x * Constants::SPRITE_SIZE + Constants::GAME_OFFSET_X, y * Constants::SPRITE_SIZE + Constants::GAME_OFFSET_Y);
            }
        }
    }

    // Render the player
    player.render();

    // Set the text colour to white
    pen(15, 15, 15);

    // Render the placeholder score text
    text("Score: 0", 2, 2);
}
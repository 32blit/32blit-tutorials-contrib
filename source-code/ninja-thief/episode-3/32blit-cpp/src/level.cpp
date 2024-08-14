#include "level.hpp"

using namespace blit;

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
    // Render border
    render_border();

    // Render platform tiles
    for (uint8_t y = 0; y < Constants::GAME_HEIGHT_TILES; y++) {
        for (uint8_t x = 0; x < Constants::GAME_WIDTH_TILES; x++) {

            // Calculate tile index
            uint8_t tile_id = level_data.platforms[y * Constants::GAME_WIDTH_TILES + x];

            // Only render the tile if it isn't a blank tile
            if (tile_id != Constants::Sprites::BLANK_TILE) {
                // Offset the tiles since the 32blit version has borders on the screen
                screen.sprite(tile_id, Point(x * Constants::SPRITE_SIZE + Constants::GAME_OFFSET_X, y * Constants::SPRITE_SIZE + Constants::GAME_OFFSET_Y));
            }
        }
    }

    // Render the player
    player.render();

    // Set the text colour to white
    screen.pen = Pen(255, 255, 255);

    // Render the placeholder score text
    screen.text("Score: 0", minimal_font, Point(2, 2));
}

void Level::render_border() {
    // Render border (only needed for 32blit, with the wider screen)

    // Each row is the same
    for (uint8_t y = 0; y < Constants::SCREEN_HEIGHT; y += Constants::SPRITE_SIZE) {
        // Left border:
        uint8_t x = 0;

        // BORDER_FULL sprites
        while (x < Constants::GAME_OFFSET_X - Constants::SPRITE_SIZE) {
            screen.sprite(Constants::Sprites::BORDER_FULL, Point(x, y));
            x += Constants::SPRITE_SIZE;
        }

        // BORDER_LEFT sprite
        screen.sprite(Constants::Sprites::BORDER_LEFT, Point(x, y));

        // Right border:
        x = Constants::SCREEN_WIDTH;

        // BORDER_FULL sprites
        while (x > Constants::SCREEN_WIDTH - Constants::GAME_OFFSET_X) {
            screen.sprite(Constants::Sprites::BORDER_FULL, Point(x, y));
            x -= Constants::SPRITE_SIZE;
        }

        // BORDER_RIGHT sprite
        screen.sprite(Constants::Sprites::BORDER_RIGHT, Point(x, y));
    }
}
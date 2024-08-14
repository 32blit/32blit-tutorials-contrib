#include "level.hpp"

using namespace picosystem;

Level::Level() {

}

Level::Level(uint8_t _level_number) {
    level_number = _level_number;
    level_data = Constants::LEVELS[level_number];

    // Search for player spawn position and create PlayerNinja object
    // Search for enemy spawn positions and create EnemyNinja objects and add them to a vector

    for (uint8_t y = 0; y < Constants::GAME_HEIGHT_TILES; y++) {
        for (uint8_t x = 0; x < Constants::GAME_WIDTH_TILES; x++) {

            // Get spritesheet index at point (x,y)
            uint8_t spawn_id = level_data.entity_spawns[y * Constants::GAME_WIDTH_TILES + x];

            // Calculate actual position from grid position
            float position_x = x * Constants::SPRITE_SIZE;
            float position_y = y * Constants::SPRITE_SIZE;

            // Create the correct instance
            if (spawn_id == Constants::Sprites::PLAYER_IDLE) {
                player = PlayerNinja(position_x, position_y);
            }
            else if (spawn_id == Constants::Sprites::PLAYER_IDLE + Constants::Sprites::RED_OFFSET) {
                enemies.push_back(EnemyNinja(position_x, position_y));
            }
        }
    }
}
void Level::update(float dt) {
    switch (level_state) {
    case LevelState::PLAYING:

        // Update player
        player.update(dt, level_data);

        if (coins_left() == 0) {
            // No more coins left, so the player has won!
            level_state = LevelState::PLAYER_WON;

            player.set_won();
        }

        // Update enemies
        for (EnemyNinja& enemy : enemies) {
            enemy.update(dt, level_data);

            if (player.check_colliding(enemy)) {
                // Player touched an enemy, so they're dead
                level_state = LevelState::PLAYER_DEAD;

                // Trigger "jump and fall" animation before restarting level
                player.set_dead();
            }
        }

        if (player.get_y() > Constants::GAME_HEIGHT) {
            // Player has gone off the bottom of the screen, so they're dead
            level_state = LevelState::FAILED;
        }

        break;

    case LevelState::PLAYER_DEAD:

        // Update player
        player.update(dt, level_data);

        if (player.get_y() > Constants::GAME_HEIGHT) {
            // Player has gone off the bottom of the screen, so we can reset the level
            level_state = LevelState::FAILED;
        }

        break;

    case LevelState::PLAYER_WON:

        // Update player
        player.update(dt, level_data);

        if (player.finished_celebrating() || player.get_y() > Constants::GAME_HEIGHT) {
            // Player has finished doing victory jumps, or has fallen off the screen
            level_state = LevelState::COMPLETE;
        }

        break;

    default:
        break;
    }
}

void Level::render() {
    // Render background pipes
    alpha(0x8);
    render_tiles(level_data.pipes);
    alpha();

    // Render water
    render_water();

    // Render platforms
    render_tiles(level_data.platforms);

    // Render extras (coins, gems and ladders)
    render_tiles(level_data.extras);

    // Render enemies
    for (EnemyNinja& enemy : enemies) {
        enemy.render();
    }
    
    // Render player
    player.render();

    // Render UI text

    // Set the text colour to white
    pen(15, 15, 15);

    // Render level number in top left corner
    std::string level_string = "Level: " + std::to_string(level_number + 1);
    text(level_string, 2, 2);

    // Render score
    std::string score_string = "Score: " + std::to_string(player.get_score());

    // Get size of score text
    int32_t w, h;
    measure(score_string, w, h);

    // Render score in top right corner
    text(score_string, Constants::SCREEN_WIDTH - 2 - w, 2);
}

void Level::render_tiles(const uint8_t* tile_ids) {
    // Iterate through array of tile ids and render using the correct index in the spritesheet
    for (uint8_t y = 0; y < Constants::GAME_HEIGHT_TILES; y++) {
        for (uint8_t x = 0; x < Constants::GAME_WIDTH_TILES; x++) {

            // Calculate tile index
            uint8_t tile_id = tile_ids[y * Constants::GAME_WIDTH_TILES + x];

            // Only render the tile if it isn't a blank tile
            if (tile_id != Constants::Sprites::BLANK_TILE) {
                sprite(tile_id, x * Constants::SPRITE_SIZE + Constants::GAME_OFFSET_X, y * Constants::SPRITE_SIZE + Constants::GAME_OFFSET_Y);
            }
        }
    }
}

void Level::render_water() {
    for (uint8_t i = 0; i < Constants::GAME_WIDTH_TILES; i++) {
        sprite(Constants::Sprites::WATER, Constants::GAME_OFFSET_X + i * Constants::SPRITE_SIZE, Constants::GAME_OFFSET_Y + Constants::GAME_HEIGHT - Constants::SPRITE_SIZE);
    }
}

bool Level::level_failed() {
    return level_state == LevelState::FAILED;
}

bool Level::level_complete() {
    return level_state == LevelState::COMPLETE;
}

uint8_t Level::get_level_number() {
    return level_number;
}

uint8_t Level::coins_left() {
    uint8_t total = 0;

    for (uint8_t i = 0; i < Constants::GAME_WIDTH_TILES * Constants::GAME_HEIGHT_TILES; i++) {
        if (level_data.extras[i] == Constants::Sprites::COIN) {
            total++;
        }
    }

    return total;
}
#include "ninja_thief.hpp"

using namespace blit;

// Our global variables are defined here
Surface* background = nullptr;

float last_time = 0.0f;

Level level;

// Setup the game
void init() {
    // Seed the random number generator
    std::srand(std::time(0));

    // Set the resolution to 160x120
    set_screen_mode(ScreenMode::lores);

    // Load the background from assets.cpp
    background = Surface::load(asset_background);

    // Load the spritesheet from assets.cpp
    Surface* spritesheet = Surface::load(asset_spritesheet);

    // Set the current spritesheet to the one we just loaded
    screen.sprites = spritesheet;

    // Load the first level
    level = Level(0);
}

// Update the game
void update(uint32_t time) {
    // Calculate change in time (in seconds) since last frame
    float dt = (time - last_time) / 1000.0f;
    last_time = time;

    // Limit dt
    if (dt > 0.05f) {
        dt = 0.05f;
    }

    // Update level
    level.update(dt);

    if (level.level_failed()) {
        // Restart the same level
        uint8_t level_number = level.get_level_number();

        level = Level(level_number);
    }
    else if (level.level_complete()) {
        // Start the next level
        uint8_t level_number = level.get_level_number() + 1;
        level_number %= Constants::LEVEL_COUNT;

        level = Level(level_number);
    }
}

// Render the game
void render(uint32_t time) {
    // Clear the screen
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // Draw the entire background image onto the screen at (0, 0)
    screen.blit(background, Rect(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), Point(0, 0));

    // Render the level
    level.render();
}
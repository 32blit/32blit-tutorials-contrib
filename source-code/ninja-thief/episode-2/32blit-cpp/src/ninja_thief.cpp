#include "ninja_thief.hpp"

using namespace blit;

// Our global variables are defined here
Surface* background = nullptr;

float last_time = 0.0f;

// Temporary player object for testing
PlayerNinja player = PlayerNinja(50, 50);

// Setup your game here
void init() {
    // Set the resolution to 160x120
    set_screen_mode(ScreenMode::lores);

    // Load the background from assets.cpp
    background = Surface::load(asset_background);

    // Load the spritesheet from assets.cpp
    Surface* spritesheet = Surface::load(asset_spritesheet);

    // Set the current spritesheet to the one we just loaded
    screen.sprites = spritesheet;
}

// This function is called to perform rendering of the game.
void render(uint32_t time) {
    // Clear the screen
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // Draw the entire background image onto the screen at (0, 0)
    screen.blit(background, Rect(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), Point(0, 0));

    // Set the text colour to white
    screen.pen = Pen(255, 255, 255);

    // Render the placeholder score text
    screen.text("Score: 0", minimal_font, Point(2, 2));

    // Render the player
    player.render();
}

// This is called to update your game state.
void update(uint32_t time) {
    // Calculate change in time (in seconds) since last frame
    float dt = (time - last_time) / 1000.0f;
    last_time = time;

    // Limit dt
    if (dt > 0.05f) {
        dt = 0.05f;
    }

    // Update the player
    player.update(dt);
}
#include "ninja_thief.hpp"

using namespace picosystem;

// Our global variables are defined here
buffer_t* background = nullptr;

float last_time = 0.0f;

Level level;

// Setup the game
void init() {
	// Seed the random number generator
    std::srand(std::time(0));
	
	// Load the spritesheet
	buffer_t* sprites = buffer(Constants::SPRITESHEET_WIDTH, Constants::SPRITESHEET_HEIGHT, asset_spritesheet);

	// Set the current spritesheet to the one we just loaded
	spritesheet(sprites);

	// Load the background
	background = buffer(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, asset_background);

	// Load the first level
	level = Level(0);
}

// Update the game
void update(uint32_t tick) {
	// Calculate change in time (in seconds) since last frame
	// The time() function returns the time in milliseconds since the device started
	float dt = (time() - last_time) / 1000.0f;
	last_time = time();

	// Limit dt
	if (dt > 0.05f) {
		dt = 0.05f;
	}

	// Update level
	level.update(dt);
}

// Render the game
void draw(uint32_t tick) {
	// Clear the screen
	pen(0, 0, 0);
	clear();

	// Draw the entire background image onto the screen at (0, 0)
	blit(background, 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, 0, 0);

	// Render the level
	level.render();
}
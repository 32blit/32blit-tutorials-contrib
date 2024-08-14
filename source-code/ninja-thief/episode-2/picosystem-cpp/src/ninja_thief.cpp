#include "ninja_thief.hpp"

using namespace picosystem;

// Our global variables are defined here
buffer_t* background = nullptr;

float last_time = 0.0f;

// Temporary player object for testing
PlayerNinja player = PlayerNinja(50, 50);

void init() {
	// Load the spritesheet
	buffer_t* sprites = buffer(Constants::SPRITESHEET_WIDTH, Constants::SPRITESHEET_HEIGHT, asset_spritesheet);

	// Set the current spritesheet to the one we just loaded
	spritesheet(sprites);

	// Load the background
	background = buffer(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, asset_background);
}

void update(uint32_t tick) {
	// Calculate change in time (in seconds) since last frame
	// The time() function returns the time in milliseconds since the device started
	float dt = (time() - last_time) / 1000.0f;
	last_time = time();

	// Limit dt
	if (dt > 0.05f) {
		dt = 0.05f;
	}

	// Update the player
	player.update(dt);
}

void draw(uint32_t tick) {
	// Clear the screen
	pen(0, 0, 0);
	clear();

	// Draw the entire background image onto the screen at (0, 0)
	blit(background, 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, 0, 0);

	// Set the text colour to white
	pen(15, 15, 15);

	// Render the placeholder score text
	text("Score: 0", 2, 2);

	// Render the player
	player.render();
}
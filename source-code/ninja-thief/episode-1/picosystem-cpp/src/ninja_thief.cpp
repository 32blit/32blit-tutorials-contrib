#include "ninja_thief.hpp"

using namespace picosystem;

// Outside of our functions, just after the includes
buffer_t* background = nullptr;

void init() {
	// Load the spritesheet
	buffer_t* sprites = buffer(Constants::SPRITESHEET_WIDTH, Constants::SPRITESHEET_HEIGHT, asset_spritesheet);

	// Set the current spritesheet to the one we just loaded
	spritesheet(sprites);

	// Load the background
	background = buffer(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, asset_background);
}

void update(uint32_t tick) {
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

	// Example primitive shape drawing code:
	/*
	// Draw a red (filled) rectangle
	pen(15, 0, 0);
	frect(10, 10, 50, 20);

	// Draw a blue (filled) circle
	pen(0, 0, 15);
	fcircle(20, 40, 10);

	// Draw a green diagonal line from top left to bottom right
	pen(0, 15, 0);
	line(0, 0, 120, 120);
	*/

	// Draw the first blue player sprite (index 32) at (50, 50)
	sprite(32, 50, 50);
}
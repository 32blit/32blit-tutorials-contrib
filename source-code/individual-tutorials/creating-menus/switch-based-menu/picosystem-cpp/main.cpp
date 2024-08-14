#include "main.hpp"

using namespace picosystem;

// Constants

const std::vector<std::string> TITLE_MENU_OPTIONS{
    "Play",
    "Credits"
};

const std::vector<std::string> CREDITS_OPTIONS{
    "Here",
    "are",
    "some",
    "credits!",
    "", // Blank line for spacing
    "Back"
};


// Possible menu states
enum class Scene {
    TITLE,
    CREDITS,
    GAME
};

// Start the game at the title screen
Scene current_scene = Scene::TITLE;

// Global variables needed by particular scenes
uint8_t option_selected = 0;
uint8_t total_presses = 0;

// Update and render functions for each state

// Title screen
void update_title() {
    if (pressed(UP)) {
        if (option_selected > 0) {
            option_selected--;
        }
    }
    else if (pressed(DOWN)) {
        if (option_selected < TITLE_MENU_OPTIONS.size() - 1) {
            option_selected++;
        }
    }

    if (pressed(A)) {
        if (option_selected == 0) {
            // Player chose "Play"
            current_scene = Scene::GAME;
            total_presses = 0;
        }
        else if (option_selected == 1) {
            // Player chose "Credits"
            current_scene = Scene::CREDITS;
        }
    }
}
void render_title() {
    // Fill screen black
    pen(0, 0, 0);
    clear();

    pen(255, 255, 255);

    // Draw the options
    for (uint8_t i = 0; i < TITLE_MENU_OPTIONS.size(); i++) {
        text(TITLE_MENU_OPTIONS[i], 30, 19 + 20 * i);
    }

    // Indicate currently selected option
    rect(20, 20 + 20 * option_selected, 5, 5);
}

// Credits screen
void update_credits() {
    if (pressed(A)) {
        // Go back to title screen
        current_scene = Scene::TITLE;
        option_selected = 0;
    }
}
void render_credits() {
    // Fill screen black
    pen(0, 0, 0);
    clear();

    pen(15, 15, 15);

    // Draw the credits
    for (uint8_t i = 0; i < CREDITS_OPTIONS.size(); i++) {
        text(CREDITS_OPTIONS[i], 30, 19 + 10 * i);
    }

    // Indicate the back option
    rect(20, 20 + 10 * (CREDITS_OPTIONS.size() - 1), 5, 5);
}

// Main game screen
void update_game() {
    if (pressed(A)) {
        total_presses++;
    }
    if (pressed(B)) {
        current_scene = Scene::TITLE;
        option_selected = 0;
    }
}
void render_game() {
    // Fill screen with a colour depending on how many times we've pressed the button
    pen((255 - total_presses) >> 4, total_presses >> 4, 15);
    clear();

    pen(15, 15, 15);
    text("Cookies: " + std::to_string(total_presses), 30, 19);
}

// Setup your game here
void init() {
}

// Update your game here
void update(uint32_t tick) {
	// Select the correct update function
    switch (current_scene) {
    case Scene::TITLE:
        update_title();
        break;

    case Scene::CREDITS:
        update_credits();
        break;

    case Scene::GAME:
        update_game();
        break;

    default:
        break;
    }
}

// Render your game here
void draw(uint32_t tick) {
	// Select the correct render function
    switch (current_scene) {
    case Scene::TITLE:
        render_title();
        break;

    case Scene::CREDITS:
        render_credits();
        break;

    case Scene::GAME:
        render_game();
        break;

    default:
        break;
    }
}
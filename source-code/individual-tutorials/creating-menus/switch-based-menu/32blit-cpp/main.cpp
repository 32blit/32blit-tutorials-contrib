#include "main.hpp"

using namespace blit;

// Constants
const std::vector<std::string> TITLE_MENU_OPTIONS {
    "Play",
    "Credits"
};

const std::vector<std::string> CREDITS_OPTIONS {
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
    if (buttons.pressed & Button::DPAD_UP) {
        if (option_selected > 0) {
            option_selected--;
        }
    }
    else if (buttons.pressed & Button::DPAD_DOWN) {
        if (option_selected < TITLE_MENU_OPTIONS.size() - 1) {
            option_selected++;
        }
    }

    if (buttons.pressed & Button::A) {
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
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    screen.pen = Pen(255, 255, 255);

    // Draw the options
    for (uint8_t i = 0; i < TITLE_MENU_OPTIONS.size(); i++) {
        screen.text(TITLE_MENU_OPTIONS[i], minimal_font, Point(30, 19 + 20 * i));
    }

    // Indicate currently selected option
    screen.rectangle(Rect(20, 20 + 20 * option_selected, 5, 5));
}

// Credits screen
void update_credits() {
    if (buttons.pressed & Button::A) {
        // Go back to title screen
        current_scene = Scene::TITLE;
        option_selected = 0;
    }
}
void render_credits() {
    // Fill screen black
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    screen.pen = Pen(255, 255, 255);

    // Draw the credits
    for (uint8_t i = 0; i < CREDITS_OPTIONS.size(); i++) {
        screen.text(CREDITS_OPTIONS[i], minimal_font, Point(30, 19 + 10 * i));
    }

    // Indicate the back option
    screen.rectangle(Rect(20, 20 + 10 * (CREDITS_OPTIONS.size() - 1), 5, 5));
}

// Main game screen
void update_game() {
    if (buttons.pressed & Button::A) {
        total_presses++;
    }
    if (buttons.pressed & Button::B) {
        current_scene = Scene::TITLE;
        option_selected = 0;
    }
}
void render_game() {
    // Fill screen with a colour depending on how many times we've pressed the button
    screen.pen = Pen(255 - total_presses, total_presses, 255);
    screen.clear();

    screen.pen = Pen(255, 255, 255);
    screen.text("Cookies: " + std::to_string(total_presses), minimal_font, Point(30, 19));
}


// Setup your game here
void init() {
    set_screen_mode(ScreenMode::lores);
}

// Update your game here
void update(uint32_t time) {
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
void render(uint32_t time) {
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
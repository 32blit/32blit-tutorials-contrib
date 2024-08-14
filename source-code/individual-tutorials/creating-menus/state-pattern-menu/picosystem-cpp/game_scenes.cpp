#include "game_scenes.hpp"

using namespace picosystem;

void TitleScene::enter() {
	option_selected = 0;
}

void TitleScene::update() {
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
            finish(new GameScene());
        }
        else if (option_selected == 1) {
            // Player chose "Credits"
            finish(new CreditsScene());
        }
    }
}

void TitleScene::render() {
    // Fill screen black
    pen(0, 0, 0);
    clear();

    pen(15, 15, 15);

    // Draw the options
    for (uint8_t i = 0; i < TITLE_MENU_OPTIONS.size(); i++) {
        text(TITLE_MENU_OPTIONS[i], 30, 19 + 20 * i);
    }

    // Indicate currently selected option
    rect(20, 20 + 20 * option_selected, 5, 5);
}

void CreditsScene::update() {
    if (pressed(A)) {
        // Go back to title screen
        finish(new TitleScene());
    }
}

void CreditsScene::render() {
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

void GameScene::enter() {
    total_presses = 0;
}

void GameScene::update() {
    if (pressed(A)) {
        total_presses++;
    }
    if (pressed(B)) {
        finish(new TitleScene());
    }
}

void GameScene::render() {
    // Fill screen with a colour depending on how many times we've pressed the button
    pen((255 - total_presses) >> 4, total_presses >> 4, 15);
    clear();

    pen(15, 15, 15);
    text("Cookies: " + std::to_string(total_presses), 30, 19);
}
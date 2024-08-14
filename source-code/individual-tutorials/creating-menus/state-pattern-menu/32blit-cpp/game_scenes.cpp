#include "game_scenes.hpp"

using namespace blit;

void TitleScene::enter() {
	option_selected = 0;
}

void TitleScene::update() {
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

void CreditsScene::update() {
    if (buttons.pressed & Button::A) {
        // Go back to title screen
        finish(new TitleScene());
    }
}

void CreditsScene::render() {
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

void GameScene::enter() {
    total_presses = 0;
}

void GameScene::update() {
    if (buttons.pressed & Button::A) {
        total_presses++;
    }
    if (buttons.pressed & Button::B) {
        finish(new TitleScene());
    }
}

void GameScene::render() {
    // Fill screen with a colour depending on how many times we've pressed the button
    screen.pen = Pen(255 - total_presses, total_presses, 255);
    screen.clear();

    screen.pen = Pen(255, 255, 255);
    screen.text("Cookies: " + std::to_string(total_presses), minimal_font, Point(30, 19));
}
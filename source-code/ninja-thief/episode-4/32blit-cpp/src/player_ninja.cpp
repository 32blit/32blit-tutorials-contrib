#include "player_ninja.hpp"

using namespace blit;

PlayerNinja::PlayerNinja() {

}

PlayerNinja::PlayerNinja(float x, float y) : Ninja(Colour::BLUE, x, y) {

}

void PlayerNinja::update(float dt, Constants::LevelData& level_data) {
    // If nothing is pressed, the player shouldn't move
    velocity_x = 0.0f;

    // Note: "else if" isn't used, because otherwise the sprite will still move when both buttons are pressed
    // Instead, we add/subtract the velocity, so if both are pressed, nothing happens
    if (pressed(Button::DPAD_LEFT)) {
        velocity_x -= Constants::Player::MAX_SPEED;
    }
    if (pressed(Button::DPAD_RIGHT)) {
        velocity_x += Constants::Player::MAX_SPEED;
    }

    // Handle climbing
    if (can_climb) {
        bool up = pressed(Button::DPAD_UP);
        bool down = pressed(Button::DPAD_DOWN);

        if (up != down) {
            // Only one of up and down are selected
            climbing_state = up ? ClimbingState::UP : ClimbingState::DOWN;
        }
        else if (climbing_state != ClimbingState::NONE) {
            // Player has already been climbing the ladder, and either none or both of up and down are pressed
            climbing_state = ClimbingState::IDLE;
        }
    }

    // Handle jumping
    // Note that we use buttons.pressed, which only contains the buttons just pressed (since the last frame)
    if (buttons.pressed & Button::A) {
        if (can_jump) {
            jump(Constants::Player::JUMP_SPEED);
        }
    }

    Ninja::update(dt, level_data);
}
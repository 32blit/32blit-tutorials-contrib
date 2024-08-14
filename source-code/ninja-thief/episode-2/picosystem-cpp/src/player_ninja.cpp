#include "player_ninja.hpp"

using namespace picosystem;

PlayerNinja::PlayerNinja() {

}

PlayerNinja::PlayerNinja(float x, float y) : Ninja(x, y) {

}

void PlayerNinja::update(float dt) {
    // If nothing is pressed, the player shouldn't move
    velocity_x = 0.0f;

    // Note: "else if" isn't used, because otherwise the sprite will still move when both buttons are pressed
    // Instead, we add/subtract the velocity, so if both are pressed, nothing happens
    if (button(LEFT)) {
        velocity_x -= Constants::Player::MAX_SPEED;
    }
    if (button(RIGHT)) {
        velocity_x += Constants::Player::MAX_SPEED;
    }

    // Handle jumping
    // Note that we use the pressed function, which returns true if the button was just pressed (since the last frame)
    if (pressed(A)) {
        jump(Constants::Player::JUMP_SPEED);
    }

    Ninja::update(dt);

    // Temporary addition to stop player falling off bottom of screen
    if (position_y > Constants::GAME_HEIGHT - Constants::SPRITE_SIZE) {
        position_y = Constants::GAME_HEIGHT - Constants::SPRITE_SIZE;
    }
}
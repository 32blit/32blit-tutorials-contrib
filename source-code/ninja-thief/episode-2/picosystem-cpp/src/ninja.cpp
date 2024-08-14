#include "ninja.hpp"

using namespace picosystem;

Ninja::Ninja() {

}

Ninja::Ninja(float x, float y) : position_x(x), position_y(y) {

}

void Ninja::update(float dt) {
    // Before we update the position, update the velocity
    velocity_y += Constants::Environment::GRAVITY_ACCELERATION * dt;

    // Move the ninja
    position_x += velocity_x * dt;
    position_y += velocity_y * dt;

    // Update direction the ninja is facing (only if the player is moving)
    if (velocity_x < 0.0f) {
        facing_direction = HorizontalDirection::LEFT;
    }
    else if (velocity_x > 0.0f) {
        facing_direction = HorizontalDirection::RIGHT;
    }
}

void Ninja::render() {
    // If ninja is travelling left, flip the image horizontally (set the transform flags)
    uint32_t transform_flags = facing_direction == HorizontalDirection::RIGHT ? 0 : HFLIP;

    // Since we are specifying the transform flags, we need to include all the extra parameters for sprite():
    //  - number of sprites across (1)
    //  - number of sprites down (1)
    //  - width to stretch to (SPRITE_SIZE - i.e. don't stretch)
    //  - height to stretch to (SPRITE_SIZE - i.e. don't stretch)
    sprite(Constants::Sprites::PLAYER_IDLE, std::round(position_x) + Constants::GAME_OFFSET_X, std::round(position_y) + Constants::GAME_OFFSET_Y, 1, 1, Constants::SPRITE_SIZE, Constants::SPRITE_SIZE, transform_flags);
}

void Ninja::jump(float jump_speed) {
    // Upwards is negative
    velocity_y = -jump_speed;
}
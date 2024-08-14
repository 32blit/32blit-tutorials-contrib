#include "ninja.hpp"

using namespace blit;

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
    // If ninja is travelling left, flip the image horizontally
    SpriteTransform transform = facing_direction == HorizontalDirection::RIGHT ? SpriteTransform::NONE : SpriteTransform::HORIZONTAL;

    screen.sprite(Constants::Sprites::PLAYER_IDLE, Point(std::round(position_x) + Constants::GAME_OFFSET_X, std::round(position_y) + Constants::GAME_OFFSET_Y), transform);
}

void Ninja::jump(float jump_speed) {
    // Upwards is negative
    velocity_y = -jump_speed;
}
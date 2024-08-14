#pragma once

#include <cmath>

#include "32blit.hpp"

#include "constants.hpp"

class Ninja {
public:
    Ninja();
    Ninja(float x, float y);

    void update(float dt);
    void render();

protected:
    void jump(float jump_speed);

    float position_x = 0.0f;
    float position_y = 0.0f;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;

    enum class HorizontalDirection {
        LEFT = -1,
        RIGHT = 1
    };

	enum class VerticalDirection {
		UP = -1,
		DOWN = 1
	};

    HorizontalDirection facing_direction = HorizontalDirection::RIGHT;
};
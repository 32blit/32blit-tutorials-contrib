#pragma once

#include <cstdlib>

#include "ninja.hpp"
#include "constants.hpp"

class EnemyNinja : public Ninja {
public:
    EnemyNinja();
    EnemyNinja(float x, float y);

    void update(float dt, Constants::LevelData& level_data);

private:
    // Returns true if there is a platform tile which is one block below and just in front of the ninja
    // This is used to work out when the ninja reaches the end of a platform
    bool platform_ahead(Constants::LevelData& level_data);

    // Returns true if there is a ladder tile above or below the ninjas's centre
    bool ladder_above_or_below(Constants::LevelData& level_data, VerticalDirection direction);

    // Finds the tile which is at the position provided
    uint8_t tile_at_position(const uint8_t* tile_array, float x, float y);

    // Returns a boolean, with chance of being true equal to probability supplied
    bool random_bool(float probability);

    int8_t current_direction = 1;
    bool climb_next_ladder = false;

    enum class AIState {
        PATROLLING,
        CLIMBING
    };

    AIState ai_state = AIState::PATROLLING;

    float speed = 0.0f;
};
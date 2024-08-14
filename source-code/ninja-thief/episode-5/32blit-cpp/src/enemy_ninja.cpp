#include "enemy_ninja.hpp"

EnemyNinja::EnemyNinja() : Ninja() {

}

EnemyNinja::EnemyNinja(float x, float y) : Ninja(Colour::RED, x, y) {
	current_direction = std::rand() % 2 ? 1 : -1;

	speed = Constants::Enemy::MIN_SPEED + (Constants::Enemy::MAX_SPEED - Constants::Enemy::MIN_SPEED) * std::rand() / static_cast<float>(RAND_MAX);
}

void EnemyNinja::update(float dt, Constants::LevelData& level_data) {
	if (ai_state == AIState::PATROLLING) {
		if (!platform_ahead(level_data)) {
			// No platform ahead, so turn around
			current_direction = -current_direction;
		}

		velocity_x = speed * current_direction;

		if (can_climb) {
			if (climb_next_ladder) {
				// We're allowed to climb - check both directions for a ladder tile
				bool can_go_up = ladder_above_or_below(level_data, VerticalDirection::UP);
				bool can_go_down = ladder_above_or_below(level_data, VerticalDirection::DOWN);

				if (can_go_up && can_go_down) {
					// If we can go either way, pick one at random
					climbing_state = std::rand() % 2 ? ClimbingState::UP : ClimbingState::DOWN;
				}
				else if (can_go_up) {
					// Only way is up
					climbing_state = ClimbingState::UP;
				}
				else if (can_go_down) {
					// Only way is down
					climbing_state = ClimbingState::DOWN;
				}

				if (climbing_state != ClimbingState::NONE) {
					// We've now decided to climb
					ai_state = AIState::CLIMBING;

					climb_next_ladder = false;
				}
			}
		}
		else {
			// Keep "re-rolling" while we can't climb

			// Decide if we should climb the next ladder we find
			climb_next_ladder = random_bool(Constants::Enemy::CLIMB_NEXT_LADDER_CHANCE);
		}
	}

	Ninja::update(dt, level_data);

	// If we're no longer in a climbing state, switch back to patrolling
	// This will happen when the enemy reaches the bottom of a ladder, or if they fall off the ladder
	if (climbing_state == ClimbingState::NONE) {
		ai_state = AIState::PATROLLING;
	}
}

bool EnemyNinja::platform_ahead(Constants::LevelData& level_data) {
	// Get a position which would be just in front of the ninja (and one tile below them)
	float point_x = position_x + Constants::SPRITE_SIZE / 2 + current_direction * Constants::Enemy::PLATFORM_DETECTION_WIDTH / 2;
	float point_y = position_y + Constants::SPRITE_SIZE;

	// Get tile at that position
	uint8_t tile_id = tile_at_position(level_data.platforms, point_x, point_y);

	// Return true if the tile is a platform (i.e. isn't an empty tile)
	return tile_id != Constants::Sprites::BLANK_TILE;
}

bool EnemyNinja::ladder_above_or_below(Constants::LevelData& level_data, VerticalDirection direction) {
	// Get a position which would be one tile above/below the ninja
	float point_x = position_x;
	float point_y = position_y + Constants::SPRITE_SIZE * static_cast<int8_t>(direction);

	// Get tile at that position
	uint8_t tile_id = tile_at_position(level_data.extras, point_x, point_y);

	// Return true if the tile is a ladder
	return tile_id == Constants::Sprites::LADDER;
}

uint8_t EnemyNinja::tile_at_position(const uint8_t* tile_array, float x, float y) {
	// Check that the position is within the game bounds (if it isn't, return an empty tile)
	if (x < 0.0f || x > Constants::GAME_WIDTH ||
		y < 0.0f || y > Constants::GAME_HEIGHT) {
		return Constants::Sprites::BLANK_TILE;
	}

	// Get grid position of tile
	uint8_t grid_x = x / Constants::SPRITE_SIZE;
	uint8_t grid_y = y / Constants::SPRITE_SIZE;

	// If we've not returned yet, then it's safe to get the tile from the level data
	return tile_array[grid_y * Constants::GAME_WIDTH_TILES + grid_x];
}

bool EnemyNinja::random_bool(float probability) {
	return std::rand() / static_cast<float>(RAND_MAX) < probability;
}
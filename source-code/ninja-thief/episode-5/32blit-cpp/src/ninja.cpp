#include "ninja.hpp"

using namespace blit;

Ninja::Ninja() {

}

Ninja::Ninja(Colour _colour, float x, float y) : colour(_colour), position_x(x), position_y(y) {

}

void Ninja::update(float dt, Constants::LevelData& level_data) {
	// This is set to true later in the update stage, but only if the ninja is on a platform
	can_jump = false;

	// Apply gravity, but only if the ninja isn't climbing a ladder
	if (climbing_state == ClimbingState::NONE) {
		velocity_y += Constants::Environment::GRAVITY_ACCELERATION * dt;
	}

	// Move the ninja
	position_x += velocity_x * dt;
	position_y += velocity_y * dt;

	// Don't allow ninja to go off the sides
	if (position_x < -Constants::Ninja::BORDER) {
		position_x = -Constants::Ninja::BORDER;
	}
	else if (position_x > Constants::GAME_WIDTH - Constants::Ninja::BORDER - Constants::Ninja::WIDTH) {
		position_x = Constants::GAME_WIDTH - Constants::Ninja::BORDER - Constants::Ninja::WIDTH;
	}

	// Detect and resolve any collisions with platforms, ladders, coins etc, only if the ninja isn't dead
	if (!dead) {
		handle_collisions(level_data);
	}

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

	// Set sprite index (take into account sprite offset based on colour)
	uint8_t index = colour == Colour::RED ? Constants::Sprites::RED_OFFSET : 0;

	if (climbing_state == ClimbingState::NONE) {
		// Player isn't climbing
		index += Constants::Sprites::PLAYER_IDLE;
	}
	else {
		// Player is climbing
		index += Constants::Sprites::PLAYER_CLIMBING_IDLE;
	}

	screen.sprite(index, Point(std::round(position_x) + Constants::GAME_OFFSET_X, std::round(position_y) + Constants::GAME_OFFSET_Y), transform);
}

bool Ninja::check_colliding(float object_x, float object_y, uint8_t object_size) {
	return (position_x + Constants::SPRITE_SIZE - Constants::Ninja::BORDER > object_x &&
			position_x + Constants::Ninja::BORDER < object_x + object_size &&
			position_y + Constants::SPRITE_SIZE > object_y &&
			position_y < object_y + object_size);
}

bool Ninja::check_colliding(Ninja& ninja) {
	float ninja_x = ninja.get_x();
	float ninja_y = ninja.get_y();

	return (position_x + Constants::SPRITE_SIZE - Constants::Ninja::BORDER > ninja_x + Constants::Ninja::BORDER &&
			position_x + Constants::Ninja::BORDER < ninja_x + Constants::SPRITE_SIZE - Constants::Ninja::BORDER &&
			position_y + Constants::SPRITE_SIZE > ninja_y &&
			position_y < ninja_y + Constants::SPRITE_SIZE);
}

float Ninja::get_x() {
	return position_x;
}

float Ninja::get_y() {
	return position_y;
}

void Ninja::handle_collisions(Constants::LevelData& level_data) {
	// Reset can_climb flag (which then gets set by handle_ladders if the ninja is near a ladder)
	can_climb = false;

	// Get position of ninja in "grid" of tiles
	// We're relying on converting to integers to truncate and hence round down
	uint8_t x = position_x / Constants::SPRITE_SIZE;
	uint8_t y = position_y / Constants::SPRITE_SIZE;

	// Check the four tiles which the ninja might be colliding with (the top left tile is marked by the x and y previously calculated)

	// We need to check that the player is within the game area
	// If they aren't, we don't need to worry about checking for collisions
	if (x < Constants::GAME_WIDTH_TILES && y < Constants::GAME_HEIGHT_TILES && position_x >= -Constants::Ninja::BORDER && position_y >= -Constants::SPRITE_SIZE) {

		// It's possible the ninja is near the edge of the screen and we could end up checking tiles which don't exist (off the edge of the screen)
		// To avoid this issue, we use the ternary operator to vary the maximum x and y offsets
		// The minimum offset is handled by the trucation, since it will round up (rather than down) if the value is negative
		for (uint8_t y_offset = 0; y_offset < (y == Constants::GAME_HEIGHT_TILES - 1 ? 1 : 2); y_offset++) {

			for (uint8_t x_offset = 0; x_offset < (x == Constants::GAME_WIDTH_TILES - 1 ? 1 : 2); x_offset++) {
				
				// Calculate grid position of this tile
				uint8_t new_x = x + x_offset;
				uint8_t new_y = y + y_offset;

				// Handle platforms
				handle_platform(level_data, new_x, new_y);

				// Handle ladders
				handle_ladder(level_data, new_x, new_y);

				// Handle scoring
				handle_scoring(level_data, new_x, new_y);
			}
		}
	}

	// If ninja can no longer climb, reset their climbing state
	if (!can_climb) {
		climbing_state = ClimbingState::NONE;
	}

	if (climbing_state != ClimbingState::NONE) {
		// If player is on a ladder, they can jump
		can_jump = true;

		// Set velocity to 0
		velocity_x = 0.0f;
		velocity_y = 0.0f;

		// Get climbing speed, depending on whether ninja is the player or an enemy
		float climbing_speed = colour == Colour::BLUE ? Constants::Player::CLIMBING_SPEED : Constants::Enemy::CLIMBING_SPEED;

		// If player is actually climbing the ladder, set vertical velocity to be in the right direction
		if (climbing_state == ClimbingState::UP) {
			velocity_y = -climbing_speed;
		}
		else if (climbing_state == ClimbingState::DOWN) {
			velocity_y = climbing_speed;
		}
	}
}

void Ninja::handle_platform(Constants::LevelData& level_data, uint8_t x, uint8_t y) {
	// Get tile's sprite index from level data
	uint8_t tile_id = level_data.platforms[y * Constants::GAME_WIDTH_TILES + x];

	// Check the tile actually exists (check that it isn't blank)
	if (tile_id != Constants::Sprites::BLANK_TILE) {

		// Calculate the actual position of the tile from the grid position
		float tile_x = x * Constants::SPRITE_SIZE;
		float tile_y = y * Constants::SPRITE_SIZE;

		// Check if the ninja is colliding with the tile
		if (check_colliding(tile_x, tile_y, Constants::SPRITE_SIZE)) {

			// Check if this platform have a ladder in front of it
			if (level_data.extras[y * Constants::GAME_WIDTH_TILES + x] == Constants::Sprites::LADDER) {

				// Check that the ninja is not on a ladder
				if (climbing_state == ClimbingState::NONE) {

					// Check that the ninja is falling downwards
					if (velocity_y > 0.0f) {

						// Check that the ninja collided with the smaller platform hitbox
						if (position_y + Constants::SPRITE_SIZE - tile_y < Constants::ONE_WAY_PLATFORM_TOLERANCE) {
							// Set the ninja's position so that it rests on top of the platform, and reset its vertical velocity to zero
							position_y = tile_y - Constants::SPRITE_SIZE;
							velocity_y = 0.0f;

							// Allow the ninja to jump again
							can_jump = true;
						}
					}
				}
			}
			else {
				// Resolve collision by finding the direction with the least intersection
				// The value of the direction variable corresponds to:
				// 0 - left side of tile
				// 1 - top side of tile
				// 2 - right side of tile
				// 3 - bottom side of tile
				uint8_t direction = 0;

				// The starting value of least_intersection is at least the maximum possible intersection
				// The width/height of the tile is the maximum intersection possible
				float least_intersection = Constants::SPRITE_SIZE;

				// Left side of tile
				float intersection = position_x + Constants::Ninja::WIDTH + Constants::Ninja::BORDER - tile_x;
				if (intersection < least_intersection) {
					direction = 0;
					least_intersection = intersection;
				}

				// Top side of tile
				intersection = position_y + Constants::SPRITE_SIZE - tile_y;
				if (intersection < least_intersection) {
					direction = 1;
					least_intersection = intersection;
				}

				// Right side of tile
				intersection = tile_x + Constants::SPRITE_SIZE - position_x - Constants::Ninja::BORDER;
				if (intersection < least_intersection) {
					direction = 2;
					least_intersection = intersection;
				}

				// Bottom side of tile
				intersection = tile_y + Constants::SPRITE_SIZE - position_y;
				if (intersection < least_intersection) {
					direction = 3;
					least_intersection = intersection;
				}

				// Now resolve collision by moving the ninja in the direction of least intersection, by exactly the amount equal to the least intersection
				switch (direction) {
				case 0:
					// Hit the left side of a platform
					position_x -= least_intersection;
					velocity_x = 0.0f;

					break;

				case 1:
					// Landed on top of a platform
					position_y -= least_intersection;
					velocity_y = 0.0f;

					// Allow the ninja to jump again
					can_jump = true;

					// Stop the ninja from climbing
					climbing_state = ClimbingState::NONE;

					break;

				case 2:
					// Hit the right side of a platform
					position_x += least_intersection;
					velocity_x = 0.0f;

					break;

				case 3:
					// Hit the underside of a platform
					position_y += least_intersection;
					velocity_y = 0.0f;

					break;

				default:
					break;
				}
			}
		}
	}
}

void Ninja::handle_ladder(Constants::LevelData& level_data, uint8_t x, uint8_t y) {
	// Get tile's sprite index from level data
	uint8_t tile_id = level_data.extras[y * Constants::GAME_WIDTH_TILES + x];

	// Check if the tile is a ladder
	if (tile_id == Constants::Sprites::LADDER) {

		// Calculate the actual position of the tile from the grid position
		float tile_x = x * Constants::SPRITE_SIZE;
		float tile_y = y * Constants::SPRITE_SIZE;

		// Check if ninja is colliding with the tile
		if (check_colliding(tile_x, tile_y, Constants::SPRITE_SIZE)) {

			// Check that ninja is sufficiently close to ladder
			if (std::abs(tile_x - position_x) < Constants::Ninja::WIDTH / 2) {
				can_climb = true;

				// Check if ninja should be climbing or idling on ladder
				if (climbing_state != ClimbingState::NONE) {
					// Lock position to ladder
					position_x = tile_x;
				}
			}
		}
	}
}

void Ninja::handle_scoring(Constants::LevelData& level_data, uint8_t x, uint8_t y) {
	// Only implemented by PlayerNinja
}

void Ninja::jump(float jump_speed) {
    // Upwards is negative
    velocity_y = -jump_speed;

	// Reset climbing state when player jumps
	climbing_state = ClimbingState::NONE;
}
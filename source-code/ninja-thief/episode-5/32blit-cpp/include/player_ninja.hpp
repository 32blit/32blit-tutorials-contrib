#pragma once

#include "32blit.hpp"

#include "ninja.hpp"
#include "constants.hpp"

class PlayerNinja : public Ninja {
public:
	PlayerNinja();
	PlayerNinja(float x, float y);

	void update(float dt, Constants::LevelData& level_data);

	uint8_t get_score();

	void set_dead();
	void set_won();

	bool finished_celebrating();

private:
	void handle_scoring(Constants::LevelData& level_data, uint8_t x, uint8_t y);

	uint8_t score = 0;

	bool won = false;

	uint8_t celebration_jumps_remaining = Constants::Player::CELEBRATION_JUMP_COUNT;
};
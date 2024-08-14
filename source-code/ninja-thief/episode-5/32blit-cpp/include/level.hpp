#pragma once

#include <string>
#include <vector>

#include "32blit.hpp"

#include "player_ninja.hpp"
#include "enemy_ninja.hpp"
#include "constants.hpp"

class Level {
public:
	Level();
	Level(uint8_t _level_number);

	void update(float dt);
	void render();

	bool level_failed();
	bool level_complete();

	uint8_t get_level_number();

private:
	void render_tiles(const uint8_t* tile_ids);
	void render_border();
	void render_water();

	uint8_t coins_left();

	Constants::LevelData level_data = {};
	uint8_t level_number = 0;

	PlayerNinja player;
	std::vector<EnemyNinja> enemies;

	enum class LevelState {
		PLAYING,
		PLAYER_DEAD,
		PLAYER_WON,
		FAILED,
		COMPLETE
	};

	LevelState level_state = LevelState::PLAYING;
};
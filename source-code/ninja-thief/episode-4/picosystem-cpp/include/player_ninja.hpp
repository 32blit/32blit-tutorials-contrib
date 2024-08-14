#pragma once

#include "picosystem.hpp"

#include "ninja.hpp"
#include "constants.hpp"

class PlayerNinja : public Ninja {
public:
	PlayerNinja();
	PlayerNinja(float x, float y);

	void update(float dt, Constants::LevelData& level_data);
};
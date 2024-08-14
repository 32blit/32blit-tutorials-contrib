#pragma once

#include "32blit.hpp"

#include <vector>

typedef std::vector<float> HapticEffect;

class HapticManager {
public:
	void update();
	void play(HapticEffect* effect, uint8_t loops = 0);
	bool playing();

private:
	HapticEffect* current_effect = nullptr;
	uint8_t loops_remaining = 0;
	uint32_t current_position = 0;
};
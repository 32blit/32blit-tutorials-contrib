#include "haptic_manager.hpp"

void HapticManager::update() {
	if (playing()) {
		blit::vibration = current_effect->at(current_position);
		current_position++;

		if (current_position >= current_effect->size()) {
			if (loops_remaining == 0) {
				current_effect = nullptr;
			}
			else {
				loops_remaining--;
				current_position = 0;
			}
		}
	}
	else {
		blit::vibration = 0.0f;
	}
}

void HapticManager::play(HapticEffect* effect, uint8_t loops) {
	current_effect = effect;
	loops_remaining = loops;
	current_position = 0;
}

bool HapticManager::playing() {
	return current_effect != nullptr;
}
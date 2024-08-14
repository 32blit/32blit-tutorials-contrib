#include "main.hpp"

using namespace blit;

uint8_t current_wave_type = 0;

std::vector<std::string> wave_names { "Sine", "Triangle", "Saw", "Square", "Noise" };

// Setup your game here
void init() {
    volume = 0xffff * 0.3;

    channels[0].waveforms = Waveform::SINE;
    channels[0].frequency = 440;
    channels[0].attack_ms = 100;
    channels[0].decay_ms = 100;
    channels[0].sustain = 0xffff * 0.7;
    channels[0].release_ms = 100;

    channels[1].waveforms = Waveform::SINE;
    channels[1].frequency = 523;
    channels[1].attack_ms = 100;
    channels[1].decay_ms = 100;
    channels[1].sustain = 0xffff * 0.6;
    channels[1].release_ms = 100;

    channels[2].waveforms = Waveform::SINE;
    channels[2].frequency = 659;
    channels[2].attack_ms = 100;
    channels[2].decay_ms = 100;
    channels[2].sustain = 0xffff * 0.4;
    channels[2].release_ms = 100;

    channels[3].waveforms = Waveform::SINE;
    channels[3].frequency = 880;
    channels[3].attack_ms = 100;
    channels[3].decay_ms = 100;
    channels[3].sustain = 0xffff * 0.4;
    channels[3].release_ms = 100;
}

// Update your game here
void update(uint32_t time) {
    if (buttons.pressed & Button::A) {
        channels[0].trigger_attack();
    }
    else if (buttons.released & Button::A) {
        channels[0].trigger_release();
    }

    if (buttons.pressed & Button::B) {
        channels[1].trigger_attack();
    }
    else if (buttons.released & Button::B) {
        channels[1].trigger_release();
    }

    if (buttons.pressed & Button::X) {
        channels[2].trigger_attack();
    }
    else if (buttons.released & Button::X) {
        channels[2].trigger_release();
    }

    if (buttons.pressed & Button::Y) {
        channels[3].trigger_attack();
    }
    else if (buttons.released & Button::Y) {
        channels[3].trigger_release();
    }



    if (buttons.pressed & Button::DPAD_LEFT) {
        if (current_wave_type > 0) {
            current_wave_type--;
        }
        else {
            current_wave_type = wave_names.size() - 1;
        }
    }
    if (buttons.pressed & Button::DPAD_RIGHT) {
        if (current_wave_type < wave_names.size() - 1) {
            current_wave_type++;
        }
        else {
            current_wave_type = 0;
        }
    }

    for (uint8_t i = 0; i < wave_names.size() - 1; i++) {
        switch (current_wave_type) {
        case 0:
            channels[i].waveforms = Waveform::SINE;
            break;
        case 1:
            channels[i].waveforms = Waveform::TRIANGLE;
            break;
        case 2:
            channels[i].waveforms = Waveform::SAW;
            break;
        case 3:
            channels[i].waveforms = Waveform::SQUARE;
            break;
        case 4:
            channels[i].waveforms = Waveform::NOISE;
            break;
        }
    }
}

// Render your game here
void render(uint32_t time) {
    // Clear the screen
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // Display if a button is currently pressed
    screen.pen = Pen(255, 255, 255);
    screen.text("Current waveform: " + wave_names[current_wave_type], minimal_font, Point(5, 4));
}
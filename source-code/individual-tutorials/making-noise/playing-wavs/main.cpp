#include "main.hpp"

using namespace blit;

bool playing = false;

void wav_callback(AudioChannel& channel) {
    // TODO
}

// Setup your game here
void init() {
    channels[0].waveforms = Waveform::WAVE;
}

// Update your game here
void update(uint32_t time) {
    if (buttons.pressed & Button::A) {
        // Toggle between play/pause
        if (playing) { // if playing
            // TODO pause
            playing = false;
        }
        else {
            // TODO start
            playing = true;
        }
    }
    else if (buttons.pressed & Button::B) {
        // TODO: restart
    }

    // Update wav stuff here
    if (playing) {
        // Do something?
    }
}

// Render your game here
void render(uint32_t time) {
    // Clear the screen
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // Display current status
    screen.pen = Pen(255, 255, 255);
    //screen.text(mp3_channel.get_playing() ? "Currently playing." : "Currently paused.", minimal_font, Point(5, 4));
}
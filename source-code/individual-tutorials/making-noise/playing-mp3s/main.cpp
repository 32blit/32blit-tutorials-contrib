#include "main.hpp"

using namespace blit;

MP3Stream mp3_channel;

// Setup your game here
void init() {
    File::add_buffer_file("temporary-file.mp3", mp3_demo, mp3_demo_length);
    mp3_channel.load("temporary-file.mp3");
}

// Update your game here
void update(uint32_t time) {
    if (buttons.pressed & Button::A) {
        // Toggle between play/pause
        if (mp3_channel.get_playing()) {
            mp3_channel.pause();
        }
        else {
            // Play the music on channel 0, and loop forever.
            mp3_channel.play(0, MP3Stream::loop);
        }
    }
    else if (buttons.pressed & Button::B) {
        mp3_channel.restart();
    }

    mp3_channel.update();
}

// Render your game here
void render(uint32_t time) {
    // Clear the screen
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // Display current status
    screen.pen = Pen(255, 255, 255);
    screen.text(mp3_channel.get_playing() ? "Currently playing." : "Currently paused.", minimal_font, Point(5, 4));
}
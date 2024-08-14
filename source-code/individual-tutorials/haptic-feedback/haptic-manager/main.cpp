#include "main.hpp"

using namespace blit;

std::vector<HapticEffect> effects {
    {
        0.3f, 0.6f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.9f,
        0.8f, 0.7f, 0.6f, 0.5f, 0.5f, 0.4f, 0.4f, 0.4f, 0.3f, 0.3f,
        0.3f, 0.2f, 0.2f, 0.2f, 0.2f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f
    },
    {
        0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f,
        0.6f, 0.4f, 0.3f, 0.4f, 0.6f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f
    },
    {
        0.3f, 0.5f, 0.6f, 0.5f, 0.3f
    },
    {
        0.4f, 0.3f, 0.1f, 0.0f, 0.0f
    },
    {
        0.8f, 0.7f, 0.6f, 0.4f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    },
    {
        1.0f, 0.8f, 0.4f, 0.0f, 0.0f
    },
    {
        1.0f, 1.0f, 0.8f, 0.5f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    }
};

HapticManager manager;

std::string last_button = "";

// Setup your game here
void init() { }

// Update your game here
void update(uint32_t time) {
    if (buttons.pressed & Button::A) {
        manager.play(&effects[0]);
        last_button = "A";
    }
    else if (buttons.pressed & Button::B) {
        manager.play(&effects[1]);
        last_button = "B";
    }
    else if (buttons.pressed & Button::X) {
        manager.play(&effects[2]);
        last_button = "X";
    }
    else if (!manager.playing()) {
        if (pressed(Button::Y)) {
            manager.play(&effects[3]);
            last_button = "Y";
        }
        else if (pressed(Button::DPAD_UP)) {
            manager.play(&effects[4]);
            last_button = "UP";
        }
        else if (pressed(Button::DPAD_DOWN)) {
            manager.play(&effects[5]);
            last_button = "DOWN";
        }
        else if (pressed(Button::DPAD_RIGHT)) {
            manager.play(&effects[6]);
            last_button = "RIGHT";
        }
    }

    manager.update();
}

// Render your game here
void render(uint32_t time) {
    // Clear the screen
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // Show a visual representation of the current vibration amplitude
    screen.pen = Pen(255, 255, 255);
    screen.text("Amplitude: ", minimal_font, Point(5, 4));

    screen.text("Last button: " + last_button, minimal_font, Point(5, 29));

    screen.pen = Pen(1.0f - vibration, vibration, 0.0f);
    screen.rectangle(Rect(5, 15, 140 * vibration, 10));
}
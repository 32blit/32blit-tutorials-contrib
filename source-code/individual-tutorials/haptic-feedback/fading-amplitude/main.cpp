#include "main.hpp"

using namespace blit;

std::string last_button = "";

// Setup your game here
void init() {
}

// Update your game here
void update(uint32_t time) {
    vibration = std::max(vibration - 0.02f, 0.0f);

    // Rest of update code...
    // For example:

    if (buttons.pressed & Button::A) {
        vibration = 0.25f;
        last_button = "A";
    }
    else if (buttons.pressed & Button::B) {
        vibration = 0.5f;
        last_button = "B";
    }
    else if (buttons.pressed & Button::X) {
        vibration = 0.75f;
        last_button = "X";
    }
    else if (buttons.pressed & Button::Y) {
        vibration = 1.0f;
        last_button = "Y";
    }
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
#include "main.hpp"

using namespace blit;

// This effect is comprised of a lot of different examples. Feel free to experiment by modifying it!
std::vector<float> effect {
    0.3f, 0.6f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.9f,
    0.8f, 0.7f, 0.6f, 0.5f, 0.5f, 0.4f, 0.4f, 0.4f, 0.3f, 0.3f,
    0.3f, 0.2f, 0.2f, 0.2f, 0.2f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f,

    0.00f, 0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.07f, 0.08f, 0.09f,
    0.10f, 0.11f, 0.12f, 0.13f, 0.14f, 0.15f, 0.16f, 0.17f, 0.18f, 0.19f,
    0.20f, 0.21f, 0.22f, 0.23f, 0.24f, 0.25f, 0.26f, 0.27f, 0.28f, 0.29f,
    0.30f, 0.31f, 0.32f, 0.33f, 0.34f, 0.35f, 0.36f, 0.37f, 0.38f, 0.39f,
    0.40f, 0.41f, 0.42f, 0.43f, 0.44f, 0.45f, 0.46f, 0.47f, 0.48f, 0.49f,
    0.50f, 0.51f, 0.52f, 0.53f, 0.54f, 0.55f, 0.56f, 0.57f, 0.58f, 0.59f,
    0.60f, 0.61f, 0.62f, 0.63f, 0.64f, 0.65f, 0.66f, 0.67f, 0.68f, 0.69f,
    0.70f, 0.71f, 0.72f, 0.73f, 0.74f, 0.75f, 0.76f, 0.77f, 0.78f, 0.79f,
    0.80f, 0.81f, 0.82f, 0.83f, 0.84f, 0.85f, 0.86f, 0.87f, 0.88f, 0.89f,
    0.90f, 0.91f, 0.92f, 0.93f, 0.94f, 0.95f, 0.96f, 0.97f, 0.98f, 0.99f,
    1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f,
    0.99f, 0.98f, 0.97f, 0.96f, 0.95f, 0.94f, 0.93f, 0.92f, 0.91f, 0.90f,
    0.89f, 0.88f, 0.87f, 0.86f, 0.85f, 0.84f, 0.83f, 0.82f, 0.81f, 0.80f,
    0.79f, 0.78f, 0.77f, 0.76f, 0.75f, 0.74f, 0.73f, 0.72f, 0.71f, 0.70f,
    0.69f, 0.68f, 0.67f, 0.66f, 0.65f, 0.64f, 0.63f, 0.62f, 0.61f, 0.60f,
    0.59f, 0.58f, 0.57f, 0.56f, 0.55f, 0.54f, 0.53f, 0.52f, 0.51f, 0.50f,
    0.49f, 0.48f, 0.47f, 0.46f, 0.45f, 0.44f, 0.43f, 0.42f, 0.41f, 0.40f,
    0.39f, 0.38f, 0.37f, 0.36f, 0.35f, 0.34f, 0.33f, 0.32f, 0.31f, 0.30f,
    0.29f, 0.28f, 0.27f, 0.26f, 0.25f, 0.24f, 0.23f, 0.22f, 0.21f, 0.20f,
    0.19f, 0.18f, 0.17f, 0.16f, 0.15f, 0.14f, 0.13f, 0.12f, 0.11f, 0.10f,
    0.09f, 0.08f, 0.07f, 0.06f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f, 0.00f,
    0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f
};

uint32_t current_position = effect.size(); // Don't play effect when position is after the end of the array

// Setup your game here
void init() { }

// Update your game here
void update(uint32_t time) {
    if (current_position < effect.size()) {
        // Set current vibration and move on to next position in the array
        vibration = effect[current_position];
        current_position++;
    }
    else {
        // When the position is outside the array bounds, we don't play the effect
        vibration = 0.0f;
    }

    // If the player pressed A, then reset the position, causing the effect to be played.
    if (buttons.pressed & Button::A) {
        current_position = 0;
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

    screen.pen = Pen(1.0f - vibration, vibration, 0.0f);
    screen.rectangle(Rect(5, 15, 140 * vibration, 10));
}
#include "main.hpp"

using namespace picosystem;

// We still need to declare this in the global scope, but it's the only variable which we need to do this for
Scene* current_scene = nullptr;

// Setup your game here
void init() {
    current_scene = new TitleScene();
    current_scene->enter();
}

// Update your game here
void update(uint32_t tick) {
	current_scene->update();

    // Check if we need to change scenes
    if (current_scene->finished()) {
        Scene* next_scene = current_scene->next();
        
        // Clean up the old scene
        current_scene->leave();
        delete current_scene;

        // Change the scene
        current_scene = next_scene;
        current_scene->enter();
    }
}

// Render your game here
void draw(uint32_t tick) {
    current_scene->render();
}
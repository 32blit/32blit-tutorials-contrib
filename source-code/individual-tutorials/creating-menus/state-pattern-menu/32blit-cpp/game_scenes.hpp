#pragma once

#include "32blit.hpp"

#include "scene.hpp"

class TitleScene : public Scene {
public:
    void enter();
    // We don't need the leave function, so we don't implement it

    void update();
    void render();

private:
    uint8_t option_selected;

    // There are several places you could put constants like these. Best practice is probably to put them in a separate constants.hpp file.
    const std::vector<std::string> TITLE_MENU_OPTIONS{
        "Play",
        "Credits"
    };
};

class CreditsScene : public Scene {
public:
    // We don't need the enter or leave functions, so we don't implement them

    void update();
    void render();

private:
    // There are several places you could put constants like these. Best practice is probably to put them in a separate constants.hpp file.
    const std::vector<std::string> CREDITS_OPTIONS{
        "Here",
        "are",
        "some",
        "credits!",
        "", // Blank line for spacing
        "Back"
    };
};

class GameScene : public Scene {
public:
    void enter();
    // We don't need the leave function, so we don't implement it

    void update();
    void render();

private:
    uint8_t total_presses;
};

require "constants"
require "player_ninja"

-- Our global variables are defined here
background = nil

last_time = 1.0

-- Temporary player object for testing
player = PlayerNinja:new(50, 50)

-- Setup your game here
function init()
    -- Set the resolution to 160x120
    set_screen_mode(ScreenMode.lores)

    -- Load the background from assets.cpp
    background = Surface.load("assets/background.blim")

    -- Load the spritesheet
    screen.load_sprites("assets/spritesheet.blim")
end

-- This function is called to perform rendering of the game.
function render(time)
    -- Clear the screen
    screen.pen = Pen(0, 0, 0)
    screen.clear()

    -- Draw the entire background image onto the screen at (0, 0)
    screen.blit(background, Rect(0, 0, Constants.SCREEN_WIDTH, Constants.SCREEN_HEIGHT), Point(0, 0))

    -- Set the text colour to white
    screen.pen = Pen(255, 255, 255)

    -- Render the placeholder score text
    screen.text("Score: 0", minimal_font, Point(2, 2))

    -- Render the player
    player:render()
end

-- This is called to update your game state.
function update(time)
    -- Calculate change in time (in seconds) since last frame
    local dt = (time - last_time) / 1000.0
    last_time = time

    -- Limit dt
    if dt > 0.05 then
        dt = 0.05
    end

    -- Update the player
    player:update(dt)
end

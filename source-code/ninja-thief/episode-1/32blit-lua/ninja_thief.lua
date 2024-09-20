require "constants"

-- Outside of our functions, just after the includes
background = nil

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

    -- Example primitive shape drawing code:
    --[[
    -- Draw a red (filled) rectangle
    screen.pen = Pen(255, 0, 0)
    screen.rectangle(Rect(10, 10, 50, 20))

    -- Draw a blue (filled) circle
    screen.pen = Pen(0, 0, 255)
    screen.circle(Point(20, 40), 10)

    -- Draw a green diagonal line from top left to bottom right
    screen.pen = Pen(0, 255, 0)
    screen.line(Point(0, 0), Point(120, 120))
    ]]--

    -- Draw the first blue player sprite (index 32) at (50, 50)
    screen.sprite(32, Point(50, 50))
end

-- This is called to update your game state.
function update(time)

end

import constants as Constants
from player_ninja import PlayerNinja

from time import ticks_ms, ticks_diff

# Perform any initialisation here, at the start of the file

last_time = 0

# Temporary player object for testing
player = PlayerNinja(50, 50)

# Load the spritesheet
sprites = Buffer(Constants.SPRITESHEET_WIDTH, Constants.SPRITESHEET_HEIGHT, "assets/spritesheet.16bpp")

# Set the current spritesheet to the one we just loaded
spritesheet(sprites)

# Load the background
background = Buffer(Constants.SCREEN_WIDTH, Constants.SCREEN_HEIGHT, "assets/background.16bpp")

# Update the game
def update(tick):
    global last_time

    # Calculate change in time (in seconds) since last frame
    # The ticks_diff() function calculates the number of milliseconds between the two times measured
    dt = ticks_diff(ticks_ms(), last_time) / 1000
    last_time = ticks_ms()

    # Limit dt
    if dt > 0.05:
        dt = 0.05

    # Update the player
    player.update(dt)

# Render the game
def draw(tick):
    # Clear the screen
    pen(0, 0, 0)
    clear()

    # Draw the entire background image onto the screen at (0, 0)
    blit(background, 0, 0, Constants.SCREEN_WIDTH, Constants.SCREEN_HEIGHT, 0, 0)

    # Set the text colour to white
    pen(15, 15, 15)

    # Render the placeholder score text
    text("Score: 0", 2, 2)

    # Render the player
    player.render()

# Enter the main game loop
start()
import constants as Constants

# Perform any initialisation here, at the start of the file

# Load the spritesheet
sprites = Buffer(Constants.SPRITESHEET_WIDTH, Constants.SPRITESHEET_HEIGHT, "assets/spritesheet.16bpp")

# Set the current spritesheet to the one we just loaded
spritesheet(sprites)

# Load the background
background = Buffer(Constants.SCREEN_WIDTH, Constants.SCREEN_HEIGHT, "assets/background.16bpp")

# Update the game
def update(tick):
    pass

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

    # Example primitive shape drawing code:
    """
    # Draw a red (filled) rectangle
    pen(15, 0, 0)
    frect(10, 10, 50, 20)

    # Draw a blue (filled) circle
    pen(0, 0, 15)
    fcircle(20, 40, 10)

    # Draw a green diagonal line from top left to bottom right
    pen(0, 15, 0)
    line(0, 0, 120, 120)
    """

    # Draw the first blue player sprite (index 32) at (50, 50)
    sprite(32, 50, 50)

# Enter the main game loop
start()
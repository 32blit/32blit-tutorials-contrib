# Constants

TITLE_MENU_OPTIONS = [
    "Play",
    "Credits"
]

CREDITS_OPTIONS = [
    "Here",
    "are",
    "some",
    "credits!",
    "", # Blank line for spacing
    "Back"
]

# Possible menu states
class Scene:
    TITLE = 0
    CREDITS = 1
    GAME = 2

# Start the game at the title screen
current_scene = Scene.TITLE

# Global variables needed by particular scenes
option_selected = 0
total_presses = 0

# Update and render functions for each state

# Title screen
def update_title():
    global current_scene, option_selected, total_presses
    
    if pressed(UP):
        if option_selected > 0:
            option_selected -= 1
    
    elif pressed(DOWN):
        if option_selected < len(TITLE_MENU_OPTIONS) - 1:
            option_selected += 1
            

    if pressed(A):
        if option_selected == 0:
            # Player chose "Play"
            current_scene = Scene.GAME
            total_presses = 0
            
        elif option_selected == 1:
            # Player chose "Credits"
            current_scene = Scene.CREDITS
            
def render_title():
    # Fill screen black
    pen(0, 0, 0)
    clear()

    pen(15, 15, 15)

    # Draw the options
    for i in range(len(TITLE_MENU_OPTIONS)):
        text(TITLE_MENU_OPTIONS[i], 30, 19 + 20 * i)

    # Indicate currently selected option
    rect(20, 20 + 20 * option_selected, 5, 5)

# Credits screen
def update_credits():
    global current_scene, option_selected
    
    if pressed(A):
        # Go back to title screen
        current_scene = Scene.TITLE
        option_selected = 0
    
def render_credits():
    # Fill screen black
    pen(0, 0, 0)
    clear()

    pen(15, 15, 15)

    # Draw the credits
    for i in range(len(CREDITS_OPTIONS)):
        text(CREDITS_OPTIONS[i], 30, 19 + 10 * i)

    # Indicate the back option
    rect(20, 20 + 10 * (len(CREDITS_OPTIONS) - 1), 5, 5)

# Main game screen
def update_game():
    global current_scene, option_selected, total_presses
    
    if pressed(A):
        total_presses += 1
    
    if pressed(B):
        current_scene = Scene.TITLE
        option_selected = 0
    
def render_game():
    # Fill screen with a colour depending on how many times we've pressed the button
    pen((255 - total_presses) >> 4, total_presses >> 4, 15)
    clear()

    pen(15, 15, 15)
    text("Cookies: " + str(total_presses), 30, 19)

# Setup your game here


# Update your game here
def update(tick):
	# Select the correct update function
    if current_scene == Scene.TITLE:
        update_title()
    elif current_scene == Scene.CREDITS:
        update_credits()
    elif current_scene == Scene.GAME:
        update_game()

# Render your game here
def draw(tick):
	# Select the correct render function
    if current_scene == Scene.TITLE:
        render_title()
    elif current_scene == Scene.CREDITS:
        render_credits()
    elif current_scene == Scene.GAME:
        render_game()
        
# Start the game
start()
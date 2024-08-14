from scene import Scene

# There are several places you could put constants like these. Best practice is probably to put them in a separate constants.py file.
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

class TitleScene(Scene):
    def enter(self):
        self.option_selected = 0

    # We don't need the leave function, so we don't implement it

    def update(self):
        if pressed(UP):
            if self.option_selected > 0:
                self.option_selected -= 1
        
        elif pressed(DOWN):
            if self.option_selected < len(TITLE_MENU_OPTIONS) - 1:
                self.option_selected += 1
                

        if pressed(A):
            if self.option_selected == 0:
                # Player chose "Play"
                self.finish(GameScene())
                
            elif self.option_selected == 1:
                # Player chose "Credits"
                self.finish(CreditsScene())

    def render(self):
        # Fill screen black
        pen(0, 0, 0)
        clear()

        pen(15, 15, 15)

        # Draw the options
        for i in range(len(TITLE_MENU_OPTIONS)):
            text(TITLE_MENU_OPTIONS[i], 30, 19 + 20 * i)

        # Indicate currently selected option
        rect(20, 20 + 20 * self.option_selected, 5, 5)

class CreditsScene(Scene):
    # We don't need the enter or leave functions, so we don't implement them

    def update(self):
        if pressed(A):
            # Go back to title screen
            self.finish(TitleScene())

    def render(self):
        # Fill screen black
        pen(0, 0, 0)
        clear()

        pen(15, 15, 15)

        # Draw the credits
        for i in range(len(CREDITS_OPTIONS)):
            text(CREDITS_OPTIONS[i], 30, 19 + 10 * i)

        # Indicate the back option
        rect(20, 20 + 10 * (len(CREDITS_OPTIONS) - 1), 5, 5)

class GameScene(Scene):
    def enter(self):
        self.total_presses = 0

    # We don't need the leave function, so we don't implement it

    def update(self):
        if pressed(A):
            self.total_presses += 1
        
        if pressed(B):
            self.finish(TitleScene())

    def render(self):
        # Fill screen with a colour depending on how many times we've pressed the button
        pen((255 - self.total_presses) >> 4, self.total_presses >> 4, 15)
        clear()

        pen(15, 15, 15)
        text("Cookies: " + str(self.total_presses), 30, 19)
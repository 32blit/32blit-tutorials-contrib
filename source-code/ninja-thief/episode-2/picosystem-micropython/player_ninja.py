from ninja import Ninja
import constants as Constants

class PlayerNinja(Ninja):

    def __init__(self, x, y):
        super().__init__(x, y)


    def update(self, dt):
        # If nothing is pressed, the player shouldn't move
        self.velocity_x = 0.0

        # Note: "else if" isn't used, because otherwise the sprite will still move when both buttons are pressed
        # Instead, we add/subtract the velocity, so if both are pressed, nothing happens
        if button(LEFT):
            self.velocity_x -= Constants.Player.MAX_SPEED
        
        if button(RIGHT):
            self.velocity_x += Constants.Player.MAX_SPEED

        # Handle jumping
        # Note that we use the pressed function, which returns true if the button was just pressed (since the last frame)
        if pressed(A):
            self.jump(Constants.Player.JUMP_SPEED)

        super().update(dt)

        # Temporary addition to stop player falling off bottom of screen
        if self.position_y > Constants.GAME_HEIGHT - Constants.SPRITE_SIZE:
            self.position_y = Constants.GAME_HEIGHT - Constants.SPRITE_SIZE
from ninja import Ninja
import constants as Constants

class PlayerNinja(Ninja):

    def __init__(self, x, y):
        super().__init__(Ninja.Colour.BLUE, x, y)


    def update(self, dt, level_data):
        # If nothing is pressed, the player shouldn't move
        self.velocity_x = 0.0

        # Note: "else if" isn't used, because otherwise the sprite will still move when both buttons are pressed
        # Instead, we add/subtract the velocity, so if both are pressed, nothing happens
        if button(LEFT):
            self.velocity_x -= Constants.Player.MAX_SPEED
        
        if button(RIGHT):
            self.velocity_x += Constants.Player.MAX_SPEED

        # Handle climbing
        if self.can_climb:
            up = button(UP)
            down = button(DOWN)

            if up != down:
                # Only one of up and down are selected
                self.climbing_state = Ninja.ClimbingState.UP if up else Ninja.ClimbingState.DOWN
            
            elif self.climbing_state != Ninja.ClimbingState.NONE:
                # Player has already been climbing the ladder, and either none or both of up and down are pressed
                self.climbing_state = Ninja.ClimbingState.IDLE

        # Handle jumping
        # Note that we use the pressed function, which returns true if the button was just pressed (since the last frame)
        if pressed(A):
            if self.can_jump:
                self.jump(Constants.Player.JUMP_SPEED)

        super().update(dt, level_data)
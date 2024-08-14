import constants as Constants

class Ninja:
    class HorizontalDirection:
        LEFT = 0
        RIGHT = 1

    class VerticalDirection:
        UP = -1
        DOWN = 1

    def __init__(self, x, y):
        self.position_x = x
        self.position_y = y

        self.velocity_x = 0
        self.velocity_y = 0

        self.facing_direction = Ninja.HorizontalDirection.RIGHT
    
    def update(self, dt):
        # Before we update the position, update the velocity
        self.velocity_y += Constants.Environment.GRAVITY_ACCELERATION * dt

        # Move the ninja
        self.position_y += self.velocity_y * dt
        self.position_x += self.velocity_x * dt

        # Update direction the ninja is facing (only if the player is moving)
        if self.velocity_x < 0:
            self.facing_direction = Ninja.HorizontalDirection.LEFT

        elif self.velocity_x > 0:
            self.facing_direction = Ninja.HorizontalDirection.RIGHT

    def render(self):
        # If ninja is travelling left, flip the image horizontally (set the transform flags)
        transform_flags = 0 if self.facing_direction == Ninja.HorizontalDirection.RIGHT else HFLIP

        # Since we are specifying the transform flags, we need to include all the extra parameters for sprite():
        #  - number of sprites across (1)
        #  - number of sprites down (1)
        #  - width to stretch to (SPRITE_SIZE - i.e. don't stretch)
        #  - height to stretch to (SPRITE_SIZE - i.e. don't stretch)
        sprite(Constants.Sprites.PLAYER_IDLE, round(self.position_x) + Constants.GAME_OFFSET_X, round(self.position_y) + Constants.GAME_OFFSET_Y, 1, 1, Constants.SPRITE_SIZE, Constants.SPRITE_SIZE, transform_flags)

    def jump(self, jump_speed):
        # Upwards is negative
        self.velocity_y = -jump_speed
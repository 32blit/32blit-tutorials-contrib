from picosystem import *

import constants as Constants

class Ninja:

    # We're using classes as enums
    class ClimbingState:
        NONE = 0
        IDLE = 1
        UP = 2
        DOWN = 3

    class HorizontalDirection:
        LEFT = 0
        RIGHT = 1

    class VerticalDirection:
        UP = -1
        DOWN = 1

    class Colour:
        BLUE = 0
        RED = 1

    def __init__(self, colour, x, y):
        self.colour = colour

        self.position_x = x
        self.position_y = y

        self.velocity_x = 0
        self.velocity_y = 0

        self.facing_direction = Ninja.HorizontalDirection.RIGHT
        
        self.can_jump = False
        self.can_climb = False

        self.climbing_state = Ninja.ClimbingState.NONE

        self.dead = False
    
    def update(self, dt, level_data):
        # This is set to true later in the update stage, but only if the ninja is on a platform
        self.can_jump = False

        # Apply gravity, only if ninja isn't climbing a ladder
        if self.climbing_state == Ninja.ClimbingState.NONE:
            self.velocity_y += Constants.Environment.GRAVITY_ACCELERATION * dt

        # Move the ninja
        self.position_x += self.velocity_x * dt
        self.position_y += self.velocity_y * dt

        # Don't allow ninja to go off the sides
        if self.position_x < -Constants.Ninja.BORDER:
            self.position_x = -Constants.Ninja.BORDER
        
        elif self.position_x > Constants.GAME_WIDTH - Constants.Ninja.BORDER - Constants.Ninja.WIDTH:
            self.position_x = Constants.GAME_WIDTH - Constants.Ninja.BORDER - Constants.Ninja.WIDTH
        
        # Detect and resolve any collisions with platforms, ladders, coins etc, only if the ninja isn't dead
        if not self.dead:
            self.handle_collisions(level_data)

        # Update direction the ninja is facing (only if the ninja is moving)
        if self.velocity_x < 0:
            self.facing_direction = Ninja.HorizontalDirection.LEFT
        
        elif self.velocity_x > 0:
            self.facing_direction = Ninja.HorizontalDirection.RIGHT
    
    def render(self):
        # If ninja is travelling left, flip the image horizontally (set the transform flags)
        transform_flags = 0 if self.facing_direction == Ninja.HorizontalDirection.RIGHT else HFLIP

        # Set sprite index (take into account sprite offset based on colour)
        index = Constants.Sprites.RED_OFFSET if self.colour == Ninja.Colour.RED else 0

        if self.climbing_state == Ninja.ClimbingState.NONE:
            # Player isn't climbing
            index += Constants.Sprites.PLAYER_IDLE
        
        else:
            # Player is climbing
            index += Constants.Sprites.PLAYER_CLIMBING_IDLE

        # Since we are specifying the transform flags, we need to include all the extra parameters for sprite():
        #  - number of sprites across (1)
        #  - number of sprites down (1)
        #  - width to stretch to (SPRITE_SIZE - i.e. don't stretch)
        #  - height to stretch to (SPRITE_SIZE - i.e. don't stretch)
        sprite(index, round(self.position_x) + Constants.GAME_OFFSET_X, round(self.position_y) + Constants.GAME_OFFSET_Y, 1, 1, Constants.SPRITE_SIZE, Constants.SPRITE_SIZE, transform_flags)
        
    def check_object_colliding(self, object_x, object_y, object_size):
        return (self.position_x + Constants.SPRITE_SIZE - Constants.Ninja.BORDER > object_x and
                self.position_x + Constants.Ninja.BORDER < object_x + object_size and
                self.position_y + Constants.SPRITE_SIZE > object_y and
                self.position_y < object_y + object_size)

    def check_ninja_colliding(self, ninja):
        ninja_x = ninja.get_x()
        ninja_y = ninja.get_y()

        return (self.position_x + Constants.SPRITE_SIZE - Constants.Ninja.BORDER > ninja_x + Constants.Ninja.BORDER and self.position_x + Constants.Ninja.BORDER < ninja_x + Constants.SPRITE_SIZE - Constants.Ninja.BORDER and
                self.position_y + Constants.SPRITE_SIZE > ninja_y and self.position_y < ninja_y + Constants.SPRITE_SIZE)

    def get_x(self):
        return self.position_x

    def get_y(self):
        return self.position_y

    def handle_collisions(self, level_data):
        # Reset can_climb flag (which then gets set by handle_ladders if the ninja is near a ladder)
        self.can_climb = False

        # Get position of ninja in "grid" of tiles
        # We're relying on converting to integers to truncate and hence round down
        x = int(self.position_x // Constants.SPRITE_SIZE)
        y = int(self.position_y // Constants.SPRITE_SIZE)

        # We need to check that the player is within the game area
        # If they aren't, we don't need to worry about checking for collisions
        if x < Constants.GAME_WIDTH_TILES and y < Constants.GAME_HEIGHT_TILES and self.position_x >= -Constants.Ninja.BORDER and self.position_y >= -Constants.SPRITE_SIZE:

            # It's possible the ninja is near the edge of the screen and we could end up checking tiles which don't exist (off the edge of the screen)
            # To avoid this issue, we use the ternary operator to vary the maximum x and y offsets
            # The minimum offset is handled by the trucation, since it will round up (rather than down) if the value is negative
            for y_offset in range(1 if y == Constants.GAME_HEIGHT_TILES - 1 else 2):

                for x_offset in range(1 if x == Constants.GAME_WIDTH_TILES - 1 else 2):
                    
                    # Calculate grid position of this tile
                    new_x = x + x_offset
                    new_y = y + y_offset

                    # Handle platforms
                    self.handle_platform(level_data, new_x, new_y)
                    
                    # Handle ladders
                    self.handle_ladder(level_data, new_x, new_y)

                    # Handle scoring
                    self.handle_scoring(level_data, new_x, new_y)

        # If ninja can no longer climb, reset their climbing state
        if not self.can_climb:
            self.climbing_state = Ninja.ClimbingState.NONE
        
        if self.climbing_state != Ninja.ClimbingState.NONE:
            # If player is on a ladder, they can jump
            self.can_jump = True

            # Set velocity to 0
            self.velocity_x = 0
            self.velocity_y = 0

            # Get climbing speed, depending on whether ninja is the player or an enemy
            climbing_speed = Constants.Player.CLIMBING_SPEED if self.colour == Ninja.Colour.BLUE else Constants.Enemy.CLIMBING_SPEED

            # If player is actually climbing the ladder, set vertical velocity to be in the right direction
            if self.climbing_state == Ninja.ClimbingState.UP:
                self.velocity_y = -climbing_speed

            elif self.climbing_state == Ninja.ClimbingState.DOWN:
                self.velocity_y = climbing_speed

    def handle_platform(self, level_data, x, y):
        # Get tile's sprite index from level data
        tile_id = level_data.platforms[y * Constants.GAME_WIDTH_TILES + x]

        # Check the tile actually exists (check that it isn't blank)
        if tile_id != Constants.Sprites.BLANK_TILE:

            # Calculate the actual position of the tile from the grid position
            tile_x = x * Constants.SPRITE_SIZE
            tile_y = y * Constants.SPRITE_SIZE

            # Check if the ninja is colliding with the tile
            if self.check_object_colliding(tile_x, tile_y, Constants.SPRITE_SIZE):
                
                # Check if this platform have a ladder in front of it
                if level_data.extras[y * Constants.GAME_WIDTH_TILES + x] == Constants.Sprites.LADDER:

                    # Check that the ninja is not on a ladder
                    if self.climbing_state == Ninja.ClimbingState.NONE:

                        # Check that the ninja is falling downwards
                        if self.velocity_y > 0:

                            # Check that the ninja collided with the smaller platform hitbox
                            if self.position_y + Constants.SPRITE_SIZE - tile_y < Constants.ONE_WAY_PLATFORM_TOLERANCE:
                                # Set the ninja's position so that it rests on top of the platform, and reset it's vertical velocity to zero
                                self.position_y = tile_y - Constants.SPRITE_SIZE
                                self.velocity_y = 0

                                # Allow the ninja to jump again
                                self.can_jump = True
                
                else:
                    # Resolve collision by finding the direction with the least intersection
                    # The value of the direction variable corresponds to:
                    # 0 - left side of tile
                    # 1 - top side of tile
                    # 2 - right side of tile
                    # 3 - bottom side of tile
                    direction = 0

                    # The starting value of least_intersection is at least the maximum possible intersection
                    # The width/height of the tile is the maximum intersection possible
                    least_intersection = Constants.SPRITE_SIZE

                    # Check each side of the tile and find the minimum intersection

                    # Left side of tile
                    intersection = self.position_x + Constants.Ninja.WIDTH + Constants.Ninja.BORDER - tile_x
                    if intersection < least_intersection:
                        direction = 0
                        least_intersection = intersection

                    # Top side of tile
                    intersection = self.position_y + Constants.SPRITE_SIZE - tile_y
                    if intersection < least_intersection:
                        direction = 1
                        least_intersection = intersection

                    # Right side of tile
                    intersection = tile_x + Constants.SPRITE_SIZE - self.position_x - Constants.Ninja.BORDER
                    if intersection < least_intersection:
                        direction = 2
                        least_intersection = intersection

                    # Bottom side of tile
                    intersection = tile_y + Constants.SPRITE_SIZE - self.position_y
                    if intersection < least_intersection:
                        direction = 3
                        least_intersection = intersection

                    # Now resolve collision by moving the ninja in the direction of least intersection, by exactly the amount equal to the least intersection
                    if direction == 0:
                        # Hit the left side of a platform
                        self.position_x -= least_intersection
                        self.velocity_x = 0

                    elif direction == 1:
                        # Landed on top of a platform
                        self.position_y -= least_intersection
                        self.velocity_y = 0

                        # Allow the ninja to jump again
                        self.can_jump = True

                        # Stop the ninja from climbing
                        self.climbing_state = Ninja.ClimbingState.NONE

                    elif direction == 2:
                        # Hit the right side of a platform
                        self.position_x += least_intersection
                        self.velocity_x = 0

                    elif direction == 3:
                        # Hit the underside of a platform
                        self.position_y += least_intersection
                        self.velocity_y = 0

    def handle_ladder(self, level_data, x, y):
        # Get tile's sprite index from level data
        tile_id = level_data.extras[y * Constants.GAME_WIDTH_TILES + x]

        # Check if the tile is a ladder
        if tile_id == Constants.Sprites.LADDER:

            # Calculate the actual position of the tile from the grid position
            tile_x = x * Constants.SPRITE_SIZE
            tile_y = y * Constants.SPRITE_SIZE

            # Check if ninja is colliding with the tile
            if self.check_object_colliding(tile_x, tile_y, Constants.SPRITE_SIZE):
                
                # Check that ninja is sufficiently close to ladder
                if abs(tile_x - self.position_x) < Constants.Ninja.WIDTH / 2:
                    self.can_climb = True

                    # Check if ninja should be climbing or idling on ladder
                    if self.climbing_state != Ninja.ClimbingState.NONE:
                        # Lock position to ladder
                        self.position_x = tile_x

    def handle_scoring(self, level_data, x, y):
        # Only implemented by PlayerNinja
        pass

    def jump(self, jump_speed):
        # Upwards is negative
        self.velocity_y = -jump_speed

        # Reset climbing state when player jumps
        self.climbing_state = Ninja.ClimbingState.NONE
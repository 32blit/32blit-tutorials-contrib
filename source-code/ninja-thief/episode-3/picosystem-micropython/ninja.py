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
    
    def update(self, dt, level_data):
        # Apply gravity
        self.velocity_y += Constants.Environment.GRAVITY_ACCELERATION * dt

        # Move the ninja
        self.position_x += self.velocity_x * dt
        self.position_y += self.velocity_y * dt

        # Don't allow ninja to go off the sides
        if self.position_x < -Constants.Ninja.BORDER:
            self.position_x = -Constants.Ninja.BORDER
        
        elif self.position_x > Constants.GAME_WIDTH - Constants.Ninja.BORDER - Constants.Ninja.WIDTH:
            self.position_x = Constants.GAME_WIDTH - Constants.Ninja.BORDER - Constants.Ninja.WIDTH
        
        # Detect and resolve any collisions with platforms, ladders, coins etc
        self.handle_collisions(level_data)

        # Update direction the ninja is facing (only if the ninja is moving)
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

    def check_object_colliding(self, object_x, object_y, object_size):
        return (self.position_x + Constants.SPRITE_SIZE - Constants.Ninja.BORDER > object_x and
                self.position_x + Constants.Ninja.BORDER < object_x + object_size and
                self.position_y + Constants.SPRITE_SIZE > object_y and
                self.position_y < object_y + object_size)

    def handle_collisions(self, level_data):
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

                elif direction == 2:
                    # Hit the right side of a platform
                    self.position_x += least_intersection
                    self.velocity_x = 0

                elif direction == 3:
                    # Hit the underside of a platform
                    self.position_y += least_intersection
                    self.velocity_y = 0

    def jump(self, jump_speed):
        # Upwards is negative
        self.velocity_y = -jump_speed
from random import random, choice

from ninja import Ninja
import constants as Constants

class EnemyNinja(Ninja):

    class AIState:
        PATROLLING = 0
        CLIMBING = 1

    def __init__(self, x, y):
        super().__init__(Ninja.Colour.RED, x, y)

        self.current_direction = 1
        self.climb_next_ladder = False

        self.ai_state = EnemyNinja.AIState.PATROLLING

    def update(self, dt, level_data):
        if self.ai_state == EnemyNinja.AIState.PATROLLING:
            if not self.platform_ahead(level_data):
                # No platform ahead, so turn around
                self.current_direction = -self.current_direction

            self.velocity_x = Constants.Enemy.MAX_SPEED * self.current_direction

            if self.can_climb:
                if self.climb_next_ladder:
                    # We're allowed to climb - check both directions for a ladder tile
                    can_go_up = self.ladder_above_or_below(level_data, Ninja.VerticalDirection.UP)
                    can_go_down = self.ladder_above_or_below(level_data, Ninja.VerticalDirection.DOWN)

                    if can_go_up and can_go_down:
                        # If we can go either way, pick one at random
                        self.climbing_state = choice([Ninja.ClimbingState.UP, Ninja.ClimbingState.DOWN])
                    
                    elif can_go_up:
                        # Only way is up
                        self.climbing_state = Ninja.ClimbingState.UP
                    
                    elif can_go_down:
                        # Only way is down
                        self.climbing_state = Ninja.ClimbingState.DOWN
                    

                    if self.climbing_state != Ninja.ClimbingState.NONE:
                        # We've now decided to climb
                        self.ai_state = EnemyNinja.AIState.CLIMBING

                        self.climb_next_ladder = False
                
            
            else:
                # Keep "re-rolling" while we can't climb

                # Decide if we should climb the next ladder we find
                self.climb_next_ladder = self.random_bool(Constants.Enemy.CLIMB_NEXT_LADDER_CHANCE)


        super().update(dt, level_data)

        # If we're no longer in a climbing state, switch back to patrolling
        # This will happen when the enemy reaches the bottom of a ladder, or if they fall off the ladder
        if self.climbing_state == Ninja.ClimbingState.NONE:
            self.ai_state = EnemyNinja.AIState.PATROLLING

    def platform_ahead(self, level_data):
        # Get a position which would be just in front of the ninja (and one tile below them)
        point_x = self.position_x + Constants.SPRITE_SIZE / 2 + self.current_direction * Constants.Enemy.PLATFORM_DETECTION_WIDTH / 2
        point_y = self.position_y + Constants.SPRITE_SIZE

        # Get tile at that position
        tile_id = self.tile_at_position(level_data.platforms, point_x, point_y)

        # Return true if the tile is a platform (i.e. isn't an empty tile)
        return tile_id != Constants.Sprites.BLANK_TILE

    def ladder_above_or_below(self, level_data, direction):
        # Get a position which would be one tile above/below the ninja
        point_x = self.position_x
        point_y = self.position_y + Constants.SPRITE_SIZE * direction

        # Get tile at that position
        tile_id = self.tile_at_position(level_data.extras, point_x, point_y)

        # Return true if the tile is a ladder
        return tile_id == Constants.Sprites.LADDER

    def tile_at_position(self, tile_array, x, y):
        # Check that the position is within the game bounds (if it isn't, return an empty tile)
        if x < 0 or x > Constants.GAME_WIDTH or y < 0 or y > Constants.GAME_HEIGHT:
            return Constants.Sprites.BLANK_TILE

        # Get grid position of tile
        grid_x = int(x // Constants.SPRITE_SIZE)
        grid_y = int(y // Constants.SPRITE_SIZE)

        # If we've not returned yet, then it's safe to get the tile from the level data
        return tile_array[grid_y * Constants.GAME_WIDTH_TILES + grid_x]

    def random_bool(self, probability):
        return random() < probability
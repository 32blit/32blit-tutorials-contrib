from player_ninja import PlayerNinja
from enemy_ninja import EnemyNinja
import constants as Constants

class Level:
    def __init__(self, level_number):
        self.level_number = level_number
        self.level_data = Constants.LEVELS[level_number].copy()

        self.enemies = []

        # Search for player spawn position and create PlayerNinja object
        # Search for enemy spawn positions and create EnemyNinja objects and add them to a vector

        for y in range(Constants.GAME_HEIGHT_TILES):
            for x in range(Constants.GAME_WIDTH_TILES):

                # Get spritesheet index at point (x,y)
                spawn_id = self.level_data.entity_spawns[y * Constants.GAME_WIDTH_TILES + x]

                # Calculate actual position from grid position
                position_x = x * Constants.SPRITE_SIZE
                position_y = y * Constants.SPRITE_SIZE

                # Create the correct instance
                if spawn_id == Constants.Sprites.PLAYER_IDLE:
                    self.player = PlayerNinja(position_x, position_y)

                elif spawn_id == Constants.Sprites.PLAYER_IDLE + Constants.Sprites.RED_OFFSET:
                    self.enemies.append(EnemyNinja(position_x, position_y))

    def update(self, dt):
        # Update player
        self.player.update(dt, self.level_data)

        # Update enemies
        for enemy in self.enemies:
            enemy.update(dt, self.level_data)


    def render(self):
        # Render platforms
        self.render_tiles(self.level_data.platforms)

        # Render extras (coins, gems and ladders)
        self.render_tiles(self.level_data.extras)

        # Render enemies
        for enemy in self.enemies:
            enemy.render()
        
        # Render player
        self.player.render()

        # Set the text colour to white
        pen(15, 15, 15)

        # Render the placeholder score text
        text("Score: 0", 2, 2)

    def render_tiles(self, tile_ids):
        # Iterate through array of tile ids and render using the correct index in the spritesheet
        for y in range(Constants.GAME_HEIGHT_TILES):
            for x in range(Constants.GAME_WIDTH_TILES):

                # Calculate tile index
                tile_id = tile_ids[y * Constants.GAME_WIDTH_TILES + x]

                # Only render the tile if it isn't a blank tile
                if tile_id != Constants.Sprites.BLANK_TILE:
                    sprite(tile_id, x * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_X, y * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_Y)
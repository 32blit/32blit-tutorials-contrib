from player_ninja import PlayerNinja
import constants as Constants

class Level:
    def __init__(self, level_number):
        self.level_number = level_number
        self.level_data = Constants.LEVELS[level_number].copy()

        # Temporary player object for testing
        self.player = PlayerNinja(50, 50)

    def update(self, dt):
        # Update the player
        self.player.update(dt, self.level_data)

    def render(self):
        # Render platform tiles
        for y in range(Constants.GAME_HEIGHT_TILES):
            for x in range(Constants.GAME_WIDTH_TILES):
                
                # Calculate tile index
                tile_id = self.level_data.platforms[y * Constants.GAME_WIDTH_TILES + x]

                # Only render the tile if it isn't a blank tile
                if tile_id != Constants.Sprites.BLANK_TILE:
                    sprite(tile_id, x * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_X, y * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_Y)
                    
        # Render the player
        self.player.render()
    
        # Set the text colour to white
        pen(15, 15, 15)

        # Render the placeholder score text
        text("Score: 0", 2, 2)
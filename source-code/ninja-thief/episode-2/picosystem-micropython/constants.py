# Screen size in pixels
SCREEN_WIDTH = 120
SCREEN_HEIGHT = 120

# Actual game area in pixels
GAME_WIDTH = 120
GAME_HEIGHT = 120

# Each sprite on the spritesheet is 8x8 pixels
SPRITE_SIZE = 8

# Size of the spritesheet in pixels (only needed for PicoSystem)
SPRITESHEET_WIDTH = SPRITE_SIZE * 8
SPRITESHEET_HEIGHT = SPRITE_SIZE * 6

# Offset of game area from top left corner
GAME_OFFSET_X = (SCREEN_WIDTH - GAME_WIDTH) // 2
GAME_OFFSET_Y = (SCREEN_HEIGHT - GAME_HEIGHT) // 2

# Sprite data, including indices to use for rendering
class Sprites:
    # Offset of the red ninja sprites from the blue ninja sprites
    RED_OFFSET = 4

    # We'll only be using PLAYER_IDLE for now
    PLAYER_IDLE = 32
    PLAYER_CLIMBING_IDLE = 33
    PLAYER_CLIMBING_1 = 34
    PLAYER_CLIMBING_2 = 35
    PLAYER_WALKING_1 = 40
    PLAYER_WALKING_2 = 41
    PLAYER_JUMPING_UP = 42
    PLAYER_JUMPING_DOWN = 43

    LADDER = 11

    COIN = 19
    GEM = 18

# Player data such as speeds
class Player:
    # Speeds are measured in pixels per second
    MAX_SPEED = 50

    JUMP_SPEED = 125

# Environment data such as gravity strength
class Environment:
    GRAVITY_ACCELERATION = 375
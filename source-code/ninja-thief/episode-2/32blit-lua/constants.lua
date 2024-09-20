Constants = {
    -- Screen size in pixels
    SCREEN_WIDTH = 160,
    SCREEN_HEIGHT = 120,

    -- Actual game area in pixels
    GAME_WIDTH = 120,
    GAME_HEIGHT = 120,

    -- Each sprite on the spritesheet is 8x8 pixels
    SPRITE_SIZE = 8,
}

-- Offset of game area from top left corner
Constants.GAME_OFFSET_X = (Constants.SCREEN_WIDTH - Constants.GAME_WIDTH) / 2
Constants.GAME_OFFSET_Y = (Constants.SCREEN_HEIGHT - Constants.GAME_HEIGHT) / 2

-- Sprite data, including indices to use for rendering
Constants.Sprites = {
    -- Offset of the red ninja sprites from the blue ninja sprites
    RED_OFFSET = 4,

    -- We'll only be using PLAYER_IDLE for now
    PLAYER_IDLE = 32,
    PLAYER_CLIMBING_IDLE = 33,
    PLAYER_CLIMBING_1 = 34,
    PLAYER_CLIMBING_2 = 35,
    PLAYER_WALKING_1 = 40,
    PLAYER_WALKING_2 = 41,
    PLAYER_JUMPING_UP = 42,
    PLAYER_JUMPING_DOWN = 43,

    LADDER = 11,

    COIN = 19,
    GEM = 18,

    -- These will be used to draw a border either side of the screen, to make the game area 120x120
    BORDER_LEFT = 10,
    BORDER_FULL = 9,
    BORDER_RIGHT = 8,
}

-- Player data such as speeds
Constants.Player = {
    -- Speeds are measured in pixels per second
    MAX_SPEED = 50.0,

    JUMP_SPEED = 125.0,
}

-- Environment data such as gravity strength
Constants.Environment = {
    GRAVITY_ACCELERATION = 375.0,
}
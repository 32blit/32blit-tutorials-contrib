require "constants"
require "player_ninja"

Level = {
    level_data = nil,
    level_number = 0,
    
    player = nil,
}

function Level:new(level_number)
    local o = {
        level_number = level_number,
        level_data = Constants.LEVELS[level_number],

        -- Temporary player object for testing
        player = PlayerNinja:new(50, 50),
    }

    setmetatable(o, self)
    self.__index = self
    return o
end

function Level:update(dt)
    self.player:update(dt, self.level_data)
end

function Level:render()
    -- Render border
    self:render_border()

    -- Render platform tiles
    for y = 0, Constants.GAME_HEIGHT_TILES - 1 do
        for x = 0, Constants.GAME_WIDTH_TILES - 1 do
            local tile_id = self.level_data.platforms[y * Constants.GAME_WIDTH_TILES + x + 1]

            if tile_id ~= Constants.Sprites.BLANK_TILE then
                -- Offset the tiles since the 32blit version has borders on the screen
                screen.sprite(tile_id, Point(x * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_X, y * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_Y))
            end
        end
    end

    -- Render the player
    self.player:render()

    -- Set the text colour to white
    screen.pen = Pen(255, 255, 255)

    -- Render the placeholder score text
    screen.text("Score: 0", minimal_font, Point(2, 2))
end

function Level:render_border()
    -- Offset the tiles since the 32blit version has borders on the screen

    -- Each row is the same
    for y = 0, Constants.SCREEN_HEIGHT - 1, Constants.SPRITE_SIZE do
        -- Left border:
        local x = 0

        -- BORDER_FULL sprites
        while x < Constants.GAME_OFFSET_X - Constants.SPRITE_SIZE do
            screen.sprite(Constants.Sprites.BORDER_FULL, Point(x, y))
            x = x + Constants.SPRITE_SIZE
        end

        -- BORDER_LEFT sprite
        screen.sprite(Constants.Sprites.BORDER_LEFT, Point(x, y))

        -- Right border:
        x = Constants.SCREEN_WIDTH

        -- BORDER_FULL sprites
        while x > Constants.SCREEN_WIDTH - Constants.GAME_OFFSET_X do
            screen.sprite(Constants.Sprites.BORDER_FULL, Point(x, y))
            x = x - Constants.SPRITE_SIZE
        end

        -- BORDER_RIGHT sprite
        screen.sprite(Constants.Sprites.BORDER_RIGHT, Point(x, y))
    end
end
require "constants"
require "enemy_ninja"
require "player_ninja"

Level = {
    level_data = nil,
    level_number = 0,
    
    player = nil,
    enemies = {},
}

function Level:new(level_number)
    local o = {
        level_number = level_number,
        level_data = Constants.LEVELS[level_number],
        enemies = {},
    }

    setmetatable(o, self)
    self.__index = self
    
    -- Search for player spawn position and create PlayerNinja object
    -- Search for enemy spawn positions and create EnemyNinja objects and add them to a vector

    for y = 0, Constants.GAME_HEIGHT_TILES - 1 do
        for x = 0, Constants.GAME_WIDTH_TILES - 1 do

            -- Get spritesheet index at point (x,y)
            local spawn_id = o.level_data.entity_spawns[y * Constants.GAME_WIDTH_TILES + x + 1]

            -- Calculate actual position from grid position
            local position_x = x * Constants.SPRITE_SIZE
            local position_y = y * Constants.SPRITE_SIZE

            -- Create the correct instance
            if spawn_id == Constants.Sprites.PLAYER_IDLE then
                o.player = PlayerNinja:new(position_x, position_y)
            
            elseif spawn_id == Constants.Sprites.PLAYER_IDLE + Constants.Sprites.RED_OFFSET then
                o.enemies[#o.enemies + 1] = EnemyNinja:new(position_x, position_y)
            end
        end
    end

    return o
end

function Level:update(dt)
    -- Update player
    self.player:update(dt, self.level_data)

    -- Update enemies
    for i, enemy in ipairs(self.enemies) do
        enemy:update(dt, self.level_data)
    end
end

function Level:render()
    -- Render border
    self:render_border()

    -- Render platforms
    self:render_tiles(self.level_data.platforms)

    -- Render extras (coins, gems and ladders)
    self:render_tiles(self.level_data.extras)

    -- Render enemies
    for i, enemy in ipairs(self.enemies) do
        enemy:render()
    end

    -- Render the player
    self.player:render()

    -- Set the text colour to white
    screen.pen = Pen(255, 255, 255)

    -- Render the placeholder score text
    screen.text("Score: 0", minimal_font, Point(2, 2))
end

function Level:render_tiles(tile_ids)
    -- Iterate through array of tile ids and render using the correct index in the spritesheet
    for y = 0, Constants.GAME_HEIGHT_TILES - 1 do
        for x = 0, Constants.GAME_WIDTH_TILES - 1 do
            -- Iterate through array of tile ids and render using the correct index in the spritesheet
            local tile_id = tile_ids[y * Constants.GAME_WIDTH_TILES + x + 1]

            -- Only render the tile if it isn't a blank tile
            if tile_id ~= Constants.Sprites.BLANK_TILE then
                -- Offset the tiles since the 32blit version has borders on the screen
                screen.sprite(tile_id, Point(x * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_X, y * Constants.SPRITE_SIZE + Constants.GAME_OFFSET_Y))
            end
        end
    end
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
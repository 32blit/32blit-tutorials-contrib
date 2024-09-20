require "constants"
require "enemy_ninja"
require "player_ninja"

LevelState = {
    PLAYING = 0,
    PLAYER_DEAD = 1,
    PLAYER_WON = 2,
    FAILED = 3,
    COMPLETE = 4,
}

Level = {
    level_data = nil,
    level_number = 0,
    
    player = nil,
    enemies = {},

    level_state = LevelState.PLAYING,
}

function Level:new(level_number)
    local o = {
        level_number = level_number,
        level_data = {
            platforms = Constants.LEVELS[level_number].platforms,
            extras = Constants.LEVELS[level_number].extras,
            entity_spawns = Constants.LEVELS[level_number].entity_spawns,
            pipes = Constants.LEVELS[level_number].pipes,
        },
        enemies = {},
    }

    setmetatable(o, self)
    self.__index = self

    -- Copy the extras table as we'll modify it later
    local extras = {}
    for i, v in ipairs(o.level_data.extras) do
        extras[i] = v
    end

    o.level_data.extras = extras
    
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
    if self.level_state == LevelState.PLAYING then
        -- Update player
        self.player:update(dt, self.level_data)

        if self:coins_left() == 0 then
            self.level_state = LevelState.PLAYER_WON
            self.player:set_won()
        end

        -- Update enemies
        for i, enemy in ipairs(self.enemies) do
            enemy:update(dt, self.level_data)

            if self.player:check_ninja_colliding(enemy) then
                -- Player touched an enemy, so they're dead
                self.level_state = LevelState.PLAYER_DEAD

                -- Trigger "jump and fall" animation before restarting level
                self.player:set_dead()
            end
        end

        if self.player:get_y() > Constants.GAME_HEIGHT then
            -- Player has gone off the bottom of the screen, so they're dead
            self.level_state = LevelState.FAILED
        end

    elseif self.level_state == LevelState.PLAYER_DEAD then
        -- Update player
        self.player:update(dt, self.level_data)

        if self.player:get_y() > Constants.GAME_HEIGHT then
            -- Player has gone off the bottom of the screen, so we can reset the level
            self.level_state = LevelState.FAILED
        end

    elseif self.level_state == LevelState.PLAYER_WON then
        -- Update player
        self.player:update(dt, self.level_data)

        if self.player:finished_celebrating() or self.player:get_y() > Constants.GAME_HEIGHT then
            self.level_state = LevelState.COMPLETE
        end
    end
end

function Level:render()
    -- Render border
    self:render_border()

    -- Render background pipes
    screen.alpha = 0x80
    self:render_tiles(self.level_data.pipes)
    screen.alpha = 0xff

    -- Render water
    self:render_water()

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

    -- Render UI text

    -- Set the text colour to white
    screen.pen = Pen(255, 255, 255)

	-- Render level number in top left corner
	local level_string = "Level: " .. self.level_number
	screen.text(level_string, minimal_font, Point(2, 2))

	-- Render score in top right corner
	local score_string = "Score: " .. self.player:get_score()
	screen.text(score_string, minimal_font, Point(Constants.SCREEN_WIDTH - 2, 2), true, TextAlign.top_right)
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

function Level:render_water()
    for i = 0, Constants.GAME_WIDTH_TILES - 1 do
        screen.sprite(Constants.Sprites.WATER, Point(Constants.GAME_OFFSET_X + i * Constants.SPRITE_SIZE, Constants.GAME_OFFSET_Y + Constants.GAME_HEIGHT - Constants.SPRITE_SIZE))
    end
end

function Level:level_failed()
    return self.level_state == LevelState.FAILED
end

function Level:level_complete()
    return self.level_state == LevelState.COMPLETE
end

function Level:get_level_number()
    return self.level_number
end

function Level:coins_left()
    local total = 0

    for i = 1, Constants.GAME_WIDTH_TILES * Constants.GAME_HEIGHT_TILES do
        if self.level_data.extras[i] == Constants.Sprites.COIN then
            total = total + 1
        end
    end

    return total
end
require "ninja"

PlayerNinja = Ninja:new()
PlayerNinja.score = 0
PlayerNinja.won = false
PlayerNinja.celebration_jumps_remaining = Constants.Player.CELEBRATION_JUMP_COUNT

function PlayerNinja:new(x, y)
    return Ninja.new(PlayerNinja, Colour.BLUE, x, y)
end

function PlayerNinja:update(dt, level_data)
    -- If nothing is pressed, the player shouldn't move
    self.velocity_x = 0.0

    if self.won then
        -- Jump in celebration!

        if self.can_jump and self.celebration_jumps_remaining > 0 then
            self:jump(Constants.Player.CELEBRATION_JUMP_SPEED)
            self.celebration_jumps_remaining = self.celebration_jumps_remaining - 1
        end
    elseif not self.dead then
        -- Handle any buttons the user has pressed

        -- Note: "else if" isn't used, because otherwise the sprite will still move when both buttons are pressed
        -- Instead, we add/subtract the velocity, so if both are pressed, nothing happens
        if buttons.state & Button.LEFT ~= 0 then
            self.velocity_x = self.velocity_x - Constants.Player.MAX_SPEED
        end
        if buttons.state & Button.RIGHT ~= 0 then
            self.velocity_x = self.velocity_x + Constants.Player.MAX_SPEED
        end

        -- Handle climbing
        if self.can_climb then
            local up = buttons.state & Button.UP ~= 0
            local down = buttons.state & Button.DOWN ~= 0

            if up ~= down then
                -- Only one of up and down are selected
                self.climbing_state = up and ClimbingState.UP or ClimbingState.DOWN
            elseif self.climbing_state ~= ClimbingState.NONE then
                -- Player has already been climbing the ladder, and either none or both of up and down are pressed
                self.climbing_state = ClimbingState.IDLE
            end
        end

        -- Handle jumping
        -- Note that we use buttons.pressed, which only contains the buttons just pressed (since the last frame)
        if buttons.pressed & Button.A ~= 0 then
            if self.can_jump then
                self:jump(Constants.Player.JUMP_SPEED)
            end
        end
    end

    -- Call parent update method
    Ninja.update(self, dt, level_data)
end

function PlayerNinja:handle_scoring(level_data, x, y)
    -- Calculate position of tile in array
    local array_position = y * Constants.GAME_WIDTH_TILES + x + 1

    -- Get tile's sprite index from level data
    local tile_id = level_data.extras[array_position]

    -- Check the tile is a coin or gem
    if tile_id == Constants.Sprites.COIN or tile_id == Constants.Sprites.GEM then

        -- Calculate the actual position of the tile from the grid position
        local tile_x = x * Constants.SPRITE_SIZE
        local tile_y = y * Constants.SPRITE_SIZE

        -- Check if the ninja is colliding with the tile
        -- We use a smaller object_size since the coins and gems are smaller, which also means we have to offset the tile_position
        if self:check_object_colliding(tile_x + Constants.Collectable.BORDER, tile_y + Constants.Collectable.BORDER, Constants.Collectable.SIZE) then

            -- Add the correct amount of score if it's a coin or gem tile
            if tile_id == Constants.Sprites.COIN then
                self.score = self.score + Constants.Collectable.COIN_SCORE
            elseif tile_id == Constants.Sprites.GEM then
                self.score = self.score + Constants.Collectable.GEM_SCORE
            end

            -- Remove item from level data
            level_data.extras[array_position] = Constants.Sprites.BLANK_TILE
        end
    end
end

function PlayerNinja:get_score()
    return self.score
end

function PlayerNinja:set_dead()
    self.dead = true
    self:jump(Constants.Player.DEATH_JUMP_SPEED)
end

function PlayerNinja:set_won()
    self.won = true
end

function PlayerNinja:finished_celebrating()
    return self.can_jump and self.celebration_jumps_remaining == 0
end
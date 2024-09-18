require "constants"

Colour = {
    BLUE = 0,
    RED = 1,
}

HorizontalDirection = {
    LEFT = -1,
    RIGHT = 1,
}

VerticalDirection = {
	UP = -1,
	DOWN = 1,
}

ClimbingState = {
    NONE = 0,
    IDLE = 1,
    UP = 2,
    DOWN = 3,
}

Ninja = {
    colour = nil,

    position_x = 0,
    posiiton_y = 0,
    velocity_x = 0,
    velocity_y = 0,

    facing_direction = HorizontalDirection.LEFT,

    can_jump = false,
    can_climb = false,

    climbing_state = ClimbingState.NONE
}

function Ninja:new(colour, x, y)
    local o = {colour = colour, position_x = x, position_y = y}

    setmetatable(o, self)
    self.__index = self
    return o
end

function Ninja:update(dt, level_data)

    -- This is set to true later in the update stage, but only if the ninja is on a platform
	self.can_jump = false

    -- Apply gravity, but only if the ninja isn't climbing a ladder
	if self.climbing_state == ClimbingState.NONE then
        self.velocity_y = self.velocity_y + Constants.Environment.GRAVITY_ACCELERATION * dt
    end

    -- Move the ninja
    self.position_x = self.position_x + self.velocity_x * dt
    self.position_y = self.position_y + self.velocity_y * dt

	-- Don't allow ninja to go off the sides
	if self.position_x < -Constants.Ninja.BORDER then
		self.position_x = -Constants.Ninja.BORDER
	elseif self.position_x > Constants.GAME_WIDTH - Constants.Ninja.BORDER - Constants.Ninja.WIDTH then
		self.position_x = Constants.GAME_WIDTH - Constants.Ninja.BORDER - Constants.Ninja.WIDTH
    end

	-- Detect and resolve any collisions with platforms, ladders, coins etc
	self:handle_collisions(level_data)

    -- Update direction the ninja is facing (only if the player is moving)
    if self.velocity_x < 0.0 then
        self.facing_direction = HorizontalDirection.LEFT
    elseif self.velocity_x > 0.0 then
        self.facing_direction = HorizontalDirection.RIGHT
    end
end

function Ninja:render()
    -- If ninja is travelling left, flip the image horizontally
    local transform = SpriteTransform.NONE
    if self.facing_direction == HorizontalDirection.LEFT then
        transform = SpriteTransform.HORIZONTAL
    end

    -- Set sprite index (take into account sprite offset based on colour)
	local index = 0

    if self.colour == Colour.RED then
        index = Constants.Sprites.RED_OFFSET
    end
    
    if self.climbing_state == ClimbingState.NONE then
        index = index + Constants.Sprites.PLAYER_IDLE
    else
        index = index + Constants.Sprites.PLAYER_CLIMBING_IDLE
    end

    screen.sprite(index, Point(math.floor(self.position_x + 0.5) + Constants.GAME_OFFSET_X, math.floor(self.position_y + 0.5) + Constants.GAME_OFFSET_Y), transform)
end

function Ninja:check_object_colliding(object_x, object_y, object_size)
    return self.position_x + Constants.SPRITE_SIZE - Constants.Ninja.BORDER > object_x and
           self.position_x + Constants.Ninja.BORDER < object_x + object_size and
           self.position_y + Constants.SPRITE_SIZE > object_y and
           self.position_y < object_y + object_size
end

function Ninja:handle_collisions(level_data)
	-- Reset can_climb flag (which then gets set by handle_ladders if the ninja is near a ladder)
    self.can_climb = false

    -- Get position of ninja in "grid" of tiles
    -- We're relying on converting to integers to truncate and hence round towards zero
    -- FIXME: this is not doing that
    local x = self.position_x // Constants.SPRITE_SIZE
    local y = self.position_y // Constants.SPRITE_SIZE

    -- Check the four tiles which the ninja might be colliding with (the top left tile is marked by the x and y previously calculated)

	-- We need to check that the player is within the game area
	-- If they aren't, we don't need to worry about checking for collisions
	if x < Constants.GAME_WIDTH_TILES and y < Constants.GAME_HEIGHT_TILES and self.position_x >= -Constants.Ninja.BORDER and self.position_y >= -Constants.SPRITE_SIZE then

		-- It's possible the ninja is near the edge of the screen and we could end up checking tiles which don't exist (off the edge of the screen)
		-- To avoid this issue, we vary the maximum x and y offsets
		-- The minimum offset is handled by the trucation, since it will round up (rather than down) if the value is negative
		local y_max = 1
        if y == Constants.GAME_HEIGHT_TILES - 1 then
            y_max = 0
        end

        for y_offset = 0, y_max do

            local x_max = 1
            if x == Constants.GAME_WIDTH_TILES - 1 then
                x_max = 0
            end

			for x_offset = 0, x_max do
				-- Calculate grid position of this tile
				local new_x = x + x_offset
				local new_y = y + y_offset

				-- Handle platforms
				self:handle_platform(level_data, new_x, new_y)

                -- Handle ladders
				self:handle_ladder(level_data, new_x, new_y)
            end
		end
	end

    -- If ninja can no longer climb, reset their climbing state
	if self.can_climb ~= true then
		self.climbing_state = ClimbingState.NONE
    end

	if self.climbing_state ~= ClimbingState.NONE then
		-- If player is on a ladder, they can jump
		self.can_jump = true

		-- Set velocity to 0
		self.velocity_x = 0.0
		self.velocity_y = 0.0

		-- Get climbing speed, depending on whether ninja is the player or an enemy
		local climbing_speed = Constants.Enemy.CLIMBING_SPEED

        if colour == Colour.BLUE then
            climbing_speed = Constants.Player.CLIMBING_SPEED
        end

		-- If player is actually climbing the ladder, set vertical velocity to be in the right direction
		if self.climbing_state == ClimbingState.UP then
			self.velocity_y = -climbing_speed
		
        elseif self.climbing_state == ClimbingState.DOWN then
			self.velocity_y = climbing_speed
        end
	end
end

function Ninja:handle_platform(level_data, x, y)
    -- Get tile's sprite index from level data
    local tile_id = level_data.platforms[y * Constants.GAME_WIDTH_TILES + x + 1]

    -- Check the tile actually exists (check that it isn't blank)
	if tile_id ~= Constants.Sprites.BLANK_TILE then

		-- Calculate the actual position of the tile from the grid position
		local tile_x = x * Constants.SPRITE_SIZE
		local tile_y = y * Constants.SPRITE_SIZE

		-- Check if the ninja is colliding with the tile
		if self:check_object_colliding(tile_x, tile_y, Constants.SPRITE_SIZE) then

            -- Check if this platform have a ladder in front of it
			if level_data.extras[y * Constants.GAME_WIDTH_TILES + x + 1] == Constants.Sprites.LADDER then

				-- Check that the ninja is not on a ladder
				if self.climbing_state == ClimbingState.NONE then

					-- Check that the ninja is falling downwards
					if self.velocity_y > 0.0 then

						-- Check that the ninja collided with the smaller platform hitbox
						if self.position_y + Constants.SPRITE_SIZE - tile_y < Constants.ONE_WAY_PLATFORM_TOLERANCE then
							-- Set the ninja's position so that it rests on top of the platform, and reset its vertical velocity to zero
							self.position_y = tile_y - Constants.SPRITE_SIZE
							self.velocity_y = 0.0

							-- Allow the ninja to jump again
							self.can_jump = true
                        end
					end
				end
			else
                -- Resolve collision by finding the direction with the least intersection
                -- The value of the direction variable corresponds to:
                -- 0 - left side of tile
                -- 1 - top side of tile
                -- 2 - right side of tile
                -- 3 - bottom side of tile
                local direction = 0

                -- The starting value of least_intersection is at least the maximum possible intersection
                -- The width/height of the tile is the maximum intersection possible
                local least_intersection = Constants.SPRITE_SIZE

                -- Left side of tile
                local intersection = self.position_x + Constants.Ninja.WIDTH + Constants.Ninja.BORDER - tile_x
                if intersection < least_intersection then
                    direction = 0
                    least_intersection = intersection
                end

                -- Top side of tile
                intersection = self.position_y + Constants.SPRITE_SIZE - tile_y
                if intersection < least_intersection then
                    direction = 1
                    least_intersection = intersection
                end

                -- Right side of tile
                intersection = tile_x + Constants.SPRITE_SIZE - self.position_x - Constants.Ninja.BORDER
                if intersection < least_intersection then
                    direction = 2
                    least_intersection = intersection
                end

                -- Bottom side of tile
                intersection = tile_y + Constants.SPRITE_SIZE - self.position_y
                if intersection < least_intersection then
                    direction = 3
                    least_intersection = intersection
                end

                -- Now resolve collision by moving the ninja in the direction of least intersection, by exactly the amount equal to the least intersection
                if direction == 0 then
                    -- Hit the left side of a platform
                    self.position_x = self.position_x - least_intersection
                    self.velocity_x = 0.0
                elseif direction == 1 then
                    -- Landed on top of a platform
                    self.position_y = self.position_y - least_intersection
                    self.velocity_y = 0.0

                    -- Allow the ninja to jump again
                    self.can_jump = true

                    -- Stop the ninja from climbing
                    self.climbing_state = ClimbingState.NONE
                elseif direction == 2 then
                    -- Hit the right side of a platform
                    self.position_x = self.position_x + least_intersection
                    self.velocity_x = 0.0
                elseif direction == 3 then
                    -- Hit the underside of a platform
                    self.position_y = self.position_y + least_intersection
                    self.velocity_y = 0.0
                end
            end
        end
    end
end

function Ninja:handle_ladder(level_data, x, y)
    -- Get tile's sprite index from level data
	local tile_id = level_data.extras[y * Constants.GAME_WIDTH_TILES + x + 1]

	-- Check if the tile is a ladder
	if tile_id == Constants.Sprites.LADDER then

		-- Calculate the actual position of the tile from the grid position
		local tile_x = x * Constants.SPRITE_SIZE
		local tile_y = y * Constants.SPRITE_SIZE

		-- Check if ninja is colliding with the tile
		if self:check_object_colliding(tile_x, tile_y, Constants.SPRITE_SIZE) then

			-- Check that ninja is sufficiently close to ladder
			if math.abs(tile_x - self.position_x) < Constants.Ninja.WIDTH / 2 then
				self.can_climb = true

				-- Check if ninja should be climbing or idling on ladder
				if self.climbing_state ~= ClimbingState.NONE then
					-- Lock position to ladder
					self.position_x = tile_x
                end
			end
		end
	end
end

function Ninja:jump(jump_speed)
    -- Upwards is negative
    self.velocity_y = -jump_speed

    -- Reset climbing state when player jumps
    self.climbing_state = ClimbingState.NONE
end


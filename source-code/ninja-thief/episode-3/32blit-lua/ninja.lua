require "constants"

HorizontalDirection = {
    LEFT = -1,
    RIGHT = 1,
}

VerticalDirection = {
	UP = -1,
	DOWN = 1,
}

Ninja = {
    position_x = 0,
    posiiton_y = 0,
    velocity_x = 0,
    velocity_y = 0,

    facing_direction = HorizontalDirection.LEFT,
}

function Ninja:new(x, y)
    local o = {position_x = x, position_y = y}

    setmetatable(o, self)
    self.__index = self
    return o
end

function Ninja:update(dt, level_data)
    -- Before we update the position, update the velocity
    self.velocity_y = self.velocity_y + Constants.Environment.GRAVITY_ACCELERATION * dt

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

    screen.sprite(Constants.Sprites.PLAYER_IDLE, Point(math.floor(self.position_x + 0.5) + Constants.GAME_OFFSET_X, math.floor(self.position_y + 0.5) + Constants.GAME_OFFSET_Y), transform)
end

function Ninja:check_object_colliding(object_x, object_y, object_size)
    return self.position_x + Constants.SPRITE_SIZE - Constants.Ninja.BORDER > object_x and
           self.position_x + Constants.Ninja.BORDER < object_x + object_size and
           self.position_y + Constants.SPRITE_SIZE > object_y and
           self.position_y < object_y + object_size
end

function Ninja:handle_collisions(level_data)
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
            end
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

function Ninja:jump(jump_speed)
    -- Upwards is negative
    self.velocity_y = -jump_speed
end


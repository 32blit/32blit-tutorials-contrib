require "ninja"

AIState = {
    PATROLLING = 0,
    CLIMBING = 1,
}

EnemyNinja = Ninja:new()
EnemyNinja.current_direction = 1
EnemyNinja.climb_next_ladder = false
EnemyNinja.ai_state = AIState.PATROLLING
EnemyNinja.speed = 0.0

function EnemyNinja:new(x, y)
    local ret = Ninja.new(EnemyNinja, Colour.RED, x, y)

	if math.random() < 0.5 then
		ret.current_direction = -1
	else
		ret.current_direction = 1
	end

	ret.speed = math.random(Constants.Enemy.MIN_SPEED, Constants.Enemy.MAX_SPEED)
	return ret
end

function EnemyNinja:update(dt, level_data)
    if self.ai_state == AIState.PATROLLING then
		if not self:platform_ahead(level_data) then
			-- No platform ahead, so turn around
			self.current_direction = -self.current_direction
		end

		self.velocity_x = self.speed * self.current_direction

		if self.can_climb then
			if self.climb_next_ladder then
				-- We're allowed to climb - check both directions for a ladder tile
				local can_go_up = self:ladder_above_or_below(level_data, VerticalDirection.UP)
				local can_go_down = self:ladder_above_or_below(level_data, VerticalDirection.DOWN)

				if can_go_up and can_go_down then
					-- If we can go either way, pick one at random
					if self:random_bool(0.5) then
						self.climbing_state = ClimbingState.UP
					else
						self.climbing_state = ClimbingState.DOWN
					end
				elseif can_go_up then
					-- Only way is up
					self.climbing_state = ClimbingState.UP
				elseif can_go_down then
					-- Only way is down
					self.climbing_state = ClimbingState.DOWN
				end

				if self.climbing_state ~= ClimbingState.NONE then
					-- We've now decided to climb
					self.ai_state = AIState.CLIMBING

					self.climb_next_ladder = false
				end
			end
		else
			-- Keep "re-rolling" while we can't climb

			-- Decide if we should climb the next ladder we find
			self.climb_next_ladder = self:random_bool(Constants.Enemy.CLIMB_NEXT_LADDER_CHANCE)
		end
    end

    Ninja.update(self, dt, level_data)

    -- If we're no longer in a climbing state, switch back to patrolling
	-- This will happen when the enemy reaches the bottom of a ladder, or if they fall off the ladder
	if self.climbing_state == ClimbingState.NONE then
		self.ai_state = AIState.PATROLLING
    end
end

-- Returns true if there is a platform tile which is one block below and just in front of the ninja
-- This is used to work out when the ninja reaches the end of a platform
function EnemyNinja:platform_ahead(level_data)
	-- Get a position which would be just in front of the ninja (and one tile below them)
	local point_x = self.position_x + Constants.SPRITE_SIZE / 2 + self.current_direction * Constants.Enemy.PLATFORM_DETECTION_WIDTH / 2
	local point_y = self.position_y + Constants.SPRITE_SIZE

	-- Get tile at that position
	local tile_id = self:tile_at_position(level_data.platforms, point_x, point_y)

	-- Return true if the tile is a platform (i.e. isn't an empty tile)
	return tile_id ~= Constants.Sprites.BLANK_TILE
end


-- Returns true if there is a ladder tile above or below the ninjas's centre
function EnemyNinja:ladder_above_or_below(level_data, direction)
	-- Get a position which would be one tile above/below the ninja
	local point_x = self.position_x
	local point_y = self.position_y + Constants.SPRITE_SIZE * direction

	-- Get tile at that position
	local tile_id = self:tile_at_position(level_data.extras, point_x, point_y)

	-- Return true if the tile is a ladder
	return tile_id == Constants.Sprites.LADDER
end

-- Finds the tile which is at the position provided
function EnemyNinja:tile_at_position(tile_array, x, y)
	-- Check that the position is within the game bounds (if it isn't, return an empty tile)
	if x < 0.0 or x > Constants.GAME_WIDTH or
		y < 0.0 or y > Constants.GAME_HEIGHT then
		return Constants.Sprites.BLANK_TILE
		end

	-- Get grid position of tile
	local grid_x = x // Constants.SPRITE_SIZE
	local grid_y = y // Constants.SPRITE_SIZE

	-- If we've not returned yet, then it's safe to get the tile from the level data
	return tile_array[grid_y * Constants.GAME_WIDTH_TILES + grid_x + 1]
end

-- Returns a boolean, with chance of being true equal to probability supplied
function EnemyNinja:random_bool(probability)
	return math.random() < probability
end




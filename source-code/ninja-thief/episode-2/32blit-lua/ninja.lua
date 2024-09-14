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

function Ninja:update(dt)
    -- Before we update the position, update the velocity
    self.velocity_y = self.velocity_y + Constants.Environment.GRAVITY_ACCELERATION * dt

    -- Move the ninja
    self.position_x = self.position_x + self.velocity_x * dt
    self.position_y = self.position_y + self.velocity_y * dt

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

function Ninja:jump(jump_speed)
    -- Upwards is negative
    self.velocity_y = -jump_speed
end

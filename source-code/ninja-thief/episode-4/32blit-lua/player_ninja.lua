require "ninja"

PlayerNinja = Ninja:new()

function PlayerNinja:new(x, y)
    return Ninja.new(PlayerNinja, Colour.BLUE, x, y)
end

function PlayerNinja:update(dt, level_data)
    -- If nothing is pressed, the player shouldn't move
    self.velocity_x = 0.0

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

    Ninja.update(self, dt, level_data)
end

require "ninja"

PlayerNinja = Ninja:new()

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

    -- Handle jumping
    -- Note that we use buttons.pressed, which only contains the buttons just pressed (since the last frame)
    if buttons.pressed & Button.A ~= 0 then
        self:jump(Constants.Player.JUMP_SPEED)
    end

    Ninja.update(self, dt, level_data)

    -- Temporary addition to stop player falling off bottom of screen
    if self.position_y > Constants.GAME_HEIGHT - Constants.SPRITE_SIZE then
        self.position_y = Constants.GAME_HEIGHT - Constants.SPRITE_SIZE
    end
end

#include "Player.h"

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
    : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
      current_ammo(max_ammo), lives(lives)
{
    // TODO: Your code here, if you want to do further initializations
}

// Move player left within the grid boundaries
void Player::move_left()
{
    // TODO: Your code here
    if (position_col > 0)
    {
        position_col--;
    }
    else
    {
        position_col = 0;
    }
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width)
{
    // TODO: Your code here
    if (position_col < grid_width - 1)
    {
        position_col++;
    }
    else
    {
        position_col = grid_width - 1;
    }
}

// Move player up within the grid boundaries
void Player::move_up()
{
    // TODO: Your code here
    if (position_row > 0)
    {
        position_row--;
    }
    else
    {
        position_row = 0;
    }
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height)
{
    // TODO: Your code here
    if (position_row < grid_height - 1)
    {
        position_row++;
    }
    else
    {
        position_row = grid_height - 1;
    }
}

// Fire if the player has ammo
bool Player::fire()
{
    if (current_ammo > 0)
    {
        current_ammo--;
        return true;
    }

    return false;
}

// Reload ammo
void Player::reload()
{
    current_ammo = max_ammo;
}

// Check if the player is alive
bool Player::is_alive() const
{
    return false;
}

// Decrease the player's lives
void Player::decrease_lives()
{
    if (lives > 0)
    {
        lives--;
    }
}

// Increase the player's lives
void Player::increase_lives()
{
    lives++;
}

int Player::get_x()
{
    return position_col;
}

int Player::get_y()
{
    return position_row;
}

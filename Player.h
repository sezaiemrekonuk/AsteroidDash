#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

using namespace std;

class Player
{
public:
    // Player's spacecraft shape
    vector<vector<bool>> spacecraft_shape;

    // Row where the top-left of the spacecraft is located
    int position_row;

    // Column where the top-left of the spacecraft is located
    int position_col;

    // Name of the player
    string player_name;

    // Maximum ammo capacity
    int max_ammo;

    // Current ammo level
    int current_ammo;

    // Current lives of the player
    int lives;

    // Constructor with default values
    Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo = 10,
           int lives = 3);

    // Move player in the space grid
    void move_left();

    // Move player to the right in the space grid
    void move_right(int grid_width);

    // Move player to the up in the space grid
    void move_up();

    // Move player to the down in the space grid
    void move_down(int grid_height);

    // Fire
    bool fire();

    // Reload ammo
    void reload();

    // Check if the player is alive
    bool is_alive() const;

    // Decrease the player's lives
    void decrease_lives();

    // Increase the player's lives
    void increase_lives();

    int get_x();
    int get_y();
};

#endif // PLAYER_H
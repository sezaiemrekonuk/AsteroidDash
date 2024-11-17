#ifndef ASTEROIDDASH_H
#define ASTEROIDDASH_H

#include <vector>
#include <string>
#include <iostream>

#include "CelestialObject.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"
#include "Player.h"

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

using namespace std;

// Class that encapsulates the game play internals
class Ammo
{
public:
    // Constructor to initialize the ammo
    Ammo(int x, int y);

    // Destructor to free the dynamically allocated space for the ammo
    ~Ammo();

    // X coordinate of the ammo
    int x;

    // Y coordinate of the ammo
    int y;

    // Pointer to the next ammo in the list
    Ammo *next_ammo = nullptr;
};

class AsteroidDash
{
public:
    // Constructor to initialize the game
    AsteroidDash(const string &space_grid_file_name, const string &celestial_objects_file_name,
                 const string &leaderboard_file_name, const string &player_file_name, const string &player_name);

    // Destructor. Remove dynamically allocated member variables here
    virtual ~AsteroidDash();

    // 2D space_grid
    vector<vector<int>> space_grid;

    // Pointer to track the player instance
    Player *player = nullptr;

    // A reference to the head of the celestial objects linked list
    CelestialObject *celestial_objects_list_head = nullptr;

    // A reference to the head of the ammo linked list
    Ammo *ammo_list_head = nullptr;

    // Current score of the game
    unsigned long current_score = 0;

    // Current game tick
    unsigned long game_time = 0;

    // Leaderboard file name to store and read the leaderboard from
    string leaderboard_file_name;

    // Leaderboard member variable
    Leaderboard leaderboard;

    // True if the game is over
    bool game_over = false;

    // Function to print the space_grid
    void print_space_grid() const;

    // Function to read the space grid from a file
    void read_space_grid(const string &input_file);

    // Function to read the player from a file
    void read_player(const string &player_file_name, const string &player_name);

    // Function to read celestial objects from a file
    // Reads the input file and calls the read_celestial_object() function for each celestial_object;
    void read_celestial_objects(const string &input_file);

    // Updates the grid based on player and celestial object states
    void update_space_grid();

    // Corresponds to the SHOOT command.
    void shoot();

    // Returns player
    Player &get_player();

    // Returns the width of the space grid
    int get_grid_width() const;

    // Returns the height of the space grid
    int get_grid_height() const;
};
#endif // ASTEROIDDASH_H

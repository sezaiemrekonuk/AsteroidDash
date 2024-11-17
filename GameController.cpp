#include "GameController.h"
#include <fstream>

// Simply instantiates the game
GameController::GameController(
    const string &space_grid_file_name,
    const string &celestial_objects_file_name,
    const string &leaderboard_file_name,
    const string &player_file_name,
    const string &player_name

)
{
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file)
{
    // TODO: Your code here
    std::ifstream file(commands_file);

    if (!file.is_open() || !file)
    {
        std::cerr << "Error: Could not open file " << commands_file << std::endl;
        return;
    }

    std::string command;
    while (std::getline(file, command))
    {
        game->update_space_grid();
        process_command(command);
        game->game_time++;
    }
}

// Processes a single command and updates game state accordingly
void GameController::process_command(const std::string &command)
{
    if (command == "PRINT_GRID")
    {
        game->print_space_grid();
    }
    else if (command == "MOVE_UP")
    {
        game->get_player().move_up();
    }
    else if (command == "MOVE_DOWN")
    {
        game->get_player().move_down(game->get_grid_height());
    }
    else if (command == "MOVE_LEFT")
    {
        game->get_player().move_left();
    }
    else if (command == "MOVE_RIGHT")
    {
        game->get_player().move_right(game->get_grid_width());
    }
    else if (command == "SHOOT")
    {
        if (game->get_player().fire())
        {
            game->shoot(); // shoot only if player has enough ammo
        }
    }
    else if (command == "NOP")
    {
        // No operation
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController()
{
    // TODO: Your code here
    delete game; // game object will handle the rest
}

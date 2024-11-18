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
        process_command(command);
        game->update_space_grid();
        game->game_time++;
    }

    game->current_score++; // last command doesnt proceed to a tick, so +1
    game->game_time--;     // last command is not a tick

    std::cout << "GAME FINISHED! No more commands!" << std::endl;
    std::cout << "Tick: " << game->game_time << std::endl;
    std::cout << "Lives: " << game->player->lives << std::endl;
    std::cout << "Ammo: " << game->player->current_ammo << std::endl;
    std::cout << "Score: " << game->current_score << std::endl;
    std::cout << "High Score: " << game->leaderboard.get_high_score() << std::endl;
    std::cout << "Player: " << game->player->player_name << std::endl;
    for (const auto &row : game->space_grid)
    {
        for (int cell : row)
        {
            std::cout << (cell == 1 ? occupiedCellChar : unoccupiedCellChar);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    game->leaderboard.print_leaderboard();
    game->update_leaderboard();
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
    game = nullptr;
}

#include "AsteroidDash.h"
#include <fstream>

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

    : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard())
{

    read_player(player_file_name, player_name);          // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);               // Initialize the grid after the player is loade
    read_celestial_objects(celestial_objects_file_name); // Load celestial objects
    // leaderboard.read_from_file(leaderboard_file_name);

    update_space_grid();
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file)
{
    // TODO: Your code here
    std::ifstream file(input_file);

    if (!file)
    {
        std::cerr << "Error: Cannot open file " << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<int> row;
        for (char c : line)
        {
            if (c == '1' || c == '0')
            {
                row.push_back(c - '0');
            }
        }
        if (!row.empty())
        {
            space_grid.push_back(row);
        }
    }
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name)
{
    // TODO: Your code here
    std::ifstream file(player_file_name);
    if (!file.is_open() || !file)
    {
        std::cerr << "Error: Could not open file " << player_file_name << std::endl;
        return;
    }

    int x, y;
    file >> x >> y;

    std::vector<std::vector<bool>> player_shape;
    std::string line;

    // skip spaces
    while (std::getline(file, line))
    {
        std::vector<bool> row;
        for (char c : line)
        {
            if (c == '1' || c == '0')
            {
                row.push_back(c == '1');
            }
        }
        if (!row.empty())
        {
            player_shape.push_back(row);
        }
    }

    player = new Player(player_shape, x, y, player_name);

    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const std::string &input_file)
{
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file)
    {
        std::cerr << "Error opening celestial objects file: " << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        if (line.empty())
        {
            continue;
        }

        std::vector<std::vector<bool>> shape;
        bool is_power_up = (line[0] == '{');
        // sec: read shape [] or {}
        while (!line.empty() && line[0] != 's' && line[0] != 'e') // stop at metadata lines
        {
            // trim whitespace to prevent infinite loops due to trailing whitespace
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

            // skipping empty lines
            if (line.empty())
            {
                if (!std::getline(file, line))
                {
                    break; // same logic below
                }
                continue;
            }

            std::vector<bool> shape_row;
            for (char c : line)
            {
                if (c == '1')
                {
                    shape_row.push_back(true);
                }
                else if (c == '0')
                {
                    shape_row.push_back(false);
                }
            }
            if (!shape_row.empty())
            {
                shape.push_back(shape_row);
            }

            if (!std::getline(file, line))
            {
                break; // break loop if no more lines for preventing infinite loops
            }
        }

        // sec2:reading metadas, assigning object type
        int start_row = -1;
        int time_of_appearance = -1;
        ObjectType object_type = ObjectType::ASTEROID;

        while (!line.empty() && (line[0] == 's' || line[0] == 't' || line[0] == 'e'))
        {
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); // trimming whitespaces because my code goes infinite loop

            if (line[0] == 's')
            {
                try
                {
                    start_row = std::stoi(line.substr(2));
                }
                catch (const std::invalid_argument &)
                {
                    std::cerr << "Invalid start row value: " << line << std::endl;
                }
            }
            else if (line[0] == 't')
            {
                try
                {
                    time_of_appearance = std::stoi(line.substr(2));
                }
                catch (const std::invalid_argument &)
                {
                    std::cerr << "Invalid time of appearance value: " << line << std::endl;
                }
            }
            else if (line[0] == 'e')
            {
                std::string effect = line.substr(2);
                if (effect == "life")
                {
                    object_type = ObjectType::LIFE_UP;
                }
                else if (effect == "ammo")
                {
                    object_type = ObjectType::AMMO;
                }
                else
                {
                    std::cerr << "Unknown effect: " << effect << std::endl;
                }
            }

            if (!std::getline(file, line))
            {
                break; // breaking the infinite loop
            }

            // if not empty, trim whitespaces again for preventing infinite loop
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        }

        CelestialObject *object = new CelestialObject(shape, object_type, start_row, time_of_appearance, false);
        // std::cout << "celestial object initialized" << std::endl;

        // sec3: adding to the linked list
        if (!celestial_objects_list_head)
        {
            celestial_objects_list_head = object;
        }
        else
        {
            CelestialObject *current = celestial_objects_list_head;
            while (current->next_celestial_object)
            {
                current = current->next_celestial_object;
            }
            current->next_celestial_object = object;
        }
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const
{
    // TODO: Your code here
    std::ofstream output_file("output.txt", std::ios::app);
    if (!output_file)
    {
        std::cerr << "Error: Cannot open output file." << std::endl;
        return;
    }

    output_file << "Tick: " << game_time << std::endl;
    output_file << "Lives: " << player->lives << std::endl;
    output_file << "Ammo: " << player->current_ammo << std::endl;
    output_file << "Score: " << current_score << std::endl;
    // output_file << "High Score: " << leaderboard.get_high_score() << std::endl;
    for (const auto &row : space_grid)
    {
        for (int cell : row)
        {
            output_file << (cell == 1 ? occupiedCellChar : unoccupiedCellChar);
        }
        output_file << std::endl;
    }

    output_file.close();
}

// Function to spawn the player in the space grid

// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid()
{
    // 1. Clear the space grid first
    for (auto &row : space_grid)
    {
        std::fill(row.begin(), row.end(), 0);
    }

    // 2. Move ammo before collision checks
    Ammo *current_ammo = ammo_list_head;
    while (current_ammo)
    {
        current_ammo->x += 1;
        current_ammo = current_ammo->next_ammo;
    }

    // 3. Check collisions between ammo and celestial objects
    current_ammo = ammo_list_head;
    while (current_ammo)
    {
        CelestialObject *current = celestial_objects_list_head;
        while (current)
        {
            if (game_time >= current->time_of_appearance)
            {
                for (int i = 0; i < current->shape.size(); ++i)
                {
                    for (int j = 0; j < current->shape[i].size(); ++j)
                    {
                        if (current->shape[i][j])
                        {
                            int row = current->starting_row + i;
                            int col = space_grid[0].size() - 1 - (game_time - current->time_of_appearance) + j;
                            if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size())
                            {
                                if (current_ammo->x == col && current_ammo->y == row)
                                {
                                    if (current->object_type == ObjectType::ASTEROID)
                                    {
                                        current->shape[i][j - 1] = false;
                                        current->generate_rotations();
                                        if (i < current->shape.size() / 2)
                                        {
                                            current->shape = current->right_rotation->shape;
                                        }
                                        else
                                        {
                                            current->shape = current->left_rotation->shape;
                                        }
                                        current_ammo->x = -1;
                                        current_ammo->y = -1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            current = current->next_celestial_object;
        }
        current_ammo = current_ammo->next_ammo;
    }

    // 4. Check collisions between player and celestial objects
    CelestialObject *current = celestial_objects_list_head;
    while (current)
    {
        if (game_time >= current->time_of_appearance)
        {
            for (int i = 0; i < current->shape.size(); ++i)
            {
                for (int j = 0; j < current->shape[i].size(); ++j)
                {
                    if (current->shape[i][j])
                    {
                        int row = current->starting_row + i;
                        int col = space_grid[0].size() - 1 - (game_time - current->time_of_appearance) + j;
                        if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size())
                        {
                            if (row >= player->get_y() && row < player->get_y() + player->spacecraft_shape.size() &&
                                col >= player->get_x() && col < player->get_x() + player->spacecraft_shape[0].size() &&
                                player->spacecraft_shape[row - player->get_y()][col - player->get_x()])
                            {
                                if (current->object_type == ObjectType::ASTEROID)
                                {
                                    player->decrease_lives();
                                    current->shape.clear();
                                }
                                else if (current->object_type == ObjectType::LIFE_UP)
                                {
                                    player->increase_lives();
                                    current->shape.clear();
                                }
                                else if (current->object_type == ObjectType::AMMO)
                                {
                                    player->reload();
                                    current->shape.clear();
                                }
                            }
                        }
                    }
                }
            }
        }
        current = current->next_celestial_object;
    }

    // 5. Clean up out-of-bounds ammo
    Ammo *prev_ammo = nullptr;
    current_ammo = ammo_list_head;
    while (current_ammo)
    {
        if (current_ammo->x < 0 || current_ammo->x >= space_grid[0].size())
        {
            if (prev_ammo)
            {
                prev_ammo->next_ammo = current_ammo->next_ammo;
                delete current_ammo;
                current_ammo = prev_ammo->next_ammo;
            }
            else
            {
                ammo_list_head = current_ammo->next_ammo;
                delete current_ammo;
                current_ammo = ammo_list_head;
            }
        }
        else
        {
            prev_ammo = current_ammo;
            current_ammo = current_ammo->next_ammo;
        }
    }

    // 6. Update display grid with current positions
    // Draw player
    for (int i = 0; i < player->spacecraft_shape.size(); ++i)
    {
        for (int j = 0; j < player->spacecraft_shape[i].size(); ++j)
        {
            if (player->spacecraft_shape[i][j])
            {
                int row = player->get_y() + i;
                int col = player->get_x() + j;
                if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size())
                {
                    space_grid[row][col] = 1;
                }
            }
        }
    }

    // Draw celestial objects
    current = celestial_objects_list_head;
    while (current)
    {
        if (game_time >= current->time_of_appearance)
        {
            for (int i = 0; i < current->shape.size(); ++i)
            {
                for (int j = 0; j < current->shape[i].size(); ++j)
                {
                    if (current->shape[i][j])
                    {
                        int row = current->starting_row + i;
                        int col = space_grid[0].size() - 1 - (game_time - current->time_of_appearance) + j;
                        if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size())
                        {
                            space_grid[row][col] = 1;
                        }
                    }
                }
            }
        }
        current = current->next_celestial_object;
    }

    // Draw ammo
    current_ammo = ammo_list_head;
    while (current_ammo)
    {
        if (current_ammo->x >= 0 && current_ammo->x < space_grid[0].size() &&
            current_ammo->y >= 0 && current_ammo->y < space_grid.size())
        {
            space_grid[current_ammo->y][current_ammo->x] = 1;
        }
        current_ammo = current_ammo->next_ammo;
    }
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot()
{
    int ammo_x = player->get_x() + player->spacecraft_shape[0].size();
    int ammo_y = player->get_y() + player->spacecraft_shape.size() / 2;

    Ammo *ammo = new Ammo(ammo_x, ammo_y);

    if (!ammo_list_head)
    {
        ammo_list_head = ammo;
    }
    else
    {
        Ammo *current = ammo_list_head;
        while (current->next_ammo)
        {
            current = current->next_ammo;
        }
        current->next_ammo = ammo;
    }
}

Player &AsteroidDash::get_player()
{
    return *player;
}

int AsteroidDash::get_grid_width() const
{
    return space_grid[0].size();
}

int AsteroidDash::get_grid_height() const
{
    return space_grid.size();
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash()
{
    // TODO: Your code here
    delete player;
    CelestialObject *current = celestial_objects_list_head;
    while (current)
    {
        CelestialObject *next = current->next_celestial_object;
        delete current;
        current = next;
    }

    Ammo *current_ammo = ammo_list_head;
    while (current_ammo)
    {
        Ammo *next = current_ammo->next_ammo;
        delete current_ammo;
        current_ammo = next;
    }
}

Ammo::Ammo(int x, int y) : x(x), y(y)
{
}

Ammo::~Ammo()
{
}

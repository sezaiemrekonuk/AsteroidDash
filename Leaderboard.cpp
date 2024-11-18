#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the leaderboard file: " << filename << std::endl;
        std::ofstream new_file(filename);
        new_file.close();
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        unsigned long score;
        time_t timestamp;
        std::string playerName;
        if (ss >> score >> timestamp)
        {
            ss.ignore(); // Skip whitespace
            std::getline(ss, playerName);
            LeaderboardEntry *newEntry = new LeaderboardEntry(score, timestamp, playerName);
            insert(newEntry);
        }
    }

    file.close();
}

// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the leaderboard file: " << filename << std::endl;
        return;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr)
    {
        file << current->score << " " << current->last_played << " " << current->player_name << "\n";
        current = current->next;
    }

    file.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard()
{
    std::cout << "Leaderboard\n-----------\n";
    LeaderboardEntry *current = head_leaderboard_entry;
    int rank = 1;
    while (current != nullptr)
    {
        struct tm *timeinfo = localtime(&current->last_played);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", timeinfo);

        std::cout << rank << ". " << current->player_name << " " << current->score << " " << buffer << "\n";
        current = current->next;
        rank++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry)
{
    if (head_leaderboard_entry == nullptr || new_entry->score > head_leaderboard_entry->score)
    {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    }
    else
    {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next != nullptr && current->next->score > new_entry->score)
        {
            current = current->next;
        }
        new_entry->next = current->next;
        current->next = new_entry;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 1;
    while (current != nullptr && current->next != nullptr)
    {
        if (count == MAX_LEADERBOARD_SIZE)
        {
            LeaderboardEntry *toDelete = current->next;
            current->next = nullptr;
            delete toDelete;
        }
        current = current->next;
        count++;
    }
}

unsigned long Leaderboard::get_high_score() const
{
    return (head_leaderboard_entry != nullptr) ? head_leaderboard_entry->score : 0;
}

void Leaderboard::set_high_score(unsigned long score)
{
    if (head_leaderboard_entry != nullptr)
    {
        head_leaderboard_entry->score = score;
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard()
{
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr)
    {
        LeaderboardEntry *toDelete = current;
        current = current->next;
        delete toDelete;
    }
}
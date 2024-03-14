#include "Leaderboard.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
        head_leaderboard_entry->next_leaderboard_entry = nullptr;
    }
    LeaderboardEntry* currentEntry = head_leaderboard_entry;
    LeaderboardEntry* beforeCurrentEntry = nullptr;


    while(currentEntry != nullptr && new_entry->score <= currentEntry->score){
        beforeCurrentEntry = currentEntry; 
        currentEntry = currentEntry->next_leaderboard_entry;
    }

    if (beforeCurrentEntry == nullptr){
        head_leaderboard_entry = new_entry;
    } else {
        beforeCurrentEntry->next_leaderboard_entry = new_entry;
    }

    new_entry->next_leaderboard_entry = currentEntry;
    leaderboardCount++;
    
    //Deleting last entry after 10
    while(leaderboardCount>10){
        currentEntry = head_leaderboard_entry;
        while (currentEntry->next_leaderboard_entry != nullptr){
            beforeCurrentEntry = currentEntry;
            currentEntry = currentEntry->next_leaderboard_entry;
        }
        beforeCurrentEntry->next_leaderboard_entry = nullptr; 
        delete currentEntry;
        leaderboardCount--;
    }
    
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    
    if (head_leaderboard_entry != nullptr) {
        std::ofstream outputFile(filename);
        if (outputFile.is_open()) {
            LeaderboardEntry* currentEntry = head_leaderboard_entry;
            int count=0;
            while (currentEntry!=nullptr){
                outputFile << currentEntry->score << " " << currentEntry->last_played << " " << currentEntry->player_name << endl;
                currentEntry = currentEntry->next_leaderboard_entry;
            }
        }
    }
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.

    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string line;
        LeaderboardEntry* currentEntry = nullptr;
        while (getline(inputFile, line)) {
            if (!line.empty()){
                std::istringstream iss(line);
                unsigned long score;
                time_t lastPlayed;
                string playerName;
                iss >> score >> lastPlayed >> playerName;
                LeaderboardEntry* inputEntry = new LeaderboardEntry(score, lastPlayed, playerName);
                if (currentEntry != nullptr){
                    currentEntry->next_leaderboard_entry = inputEntry;
                } else {
                    head_leaderboard_entry = inputEntry;
                }
                currentEntry = inputEntry;
                leaderboardCount++;
            }
        }
        inputFile.close();
    }
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    if (head_leaderboard_entry != nullptr) {
        LeaderboardEntry* currentEntry = head_leaderboard_entry;
        int count = 1;
            while (currentEntry!=nullptr){
                const int bufferSize = 64;
                char timeString[bufferSize];
                strftime(timeString, sizeof(timeString), "%H:%M:%S/%d.%m.%Y", localtime(&currentEntry->last_played));
                std::cout << count << ". "  << currentEntry->player_name  << " " << currentEntry->score << " " << timeString << std::endl;
                currentEntry = currentEntry->next_leaderboard_entry;
                count++;
            }
    }
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* current_entry = head_leaderboard_entry;
    LeaderboardEntry* next_entry;

    while (current_entry != nullptr) {
        //Move to the next entry in the linked list
        next_entry = current_entry->next_leaderboard_entry;

        //Delete the current entry
        delete current_entry;

        //Move to the next entry in the linked list
        current_entry = next_entry;
    }

    head_leaderboard_entry = nullptr;
}  

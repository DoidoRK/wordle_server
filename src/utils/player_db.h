#ifndef _PLAYER_DB_H_
#define _PLAYER_DB_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <pthread.h>
#include "../wordle_types.h"

#define PLAYER_BASE_PATH "db/players.txt"

using namespace std;

//Variables to keep track of player's scores during runtime.
map<string, int> player_database;
highscore_t highscore[HIGHSCORE_SIZE];

//Mutexes to control threads access to playerbase and highscores
pthread_mutex_t player_database_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t highscore_mutex = PTHREAD_MUTEX_INITIALIZER;

// Loads the player database from the CSV file
void loadPlayerDatabase() {
    ifstream file(PLAYER_BASE_PATH);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string player_name;
            int player_score;
            if (getline(iss, player_name, ',') && (iss >> player_score)) {
                player_database[player_name] = player_score;
            }
        }
        file.close();
    }
}

// Save the player database back to the CSV file
void savePlayerDatabase() {
    pthread_mutex_lock(&player_database_mutex);
    ofstream file(PLAYER_BASE_PATH);
    if (file.is_open()) {
        for (const auto& player : player_database) {
            file << player.first << "," << player.second << endl;
        }
        file.close();
    }
    pthread_mutex_unlock(&player_database_mutex);
}

// Check if a player is in the database, add with a score of 0 if not
int getPlayerScore(const string& player_name) {
    if (player_database.find(player_name) != player_database.end()) {
        // Player is in the database, return the score
        return player_database[player_name];
    } else {
        pthread_mutex_lock(&player_database_mutex);
        // Player is not in the database, add with a score of 0
        player_database[player_name] = 0;
        pthread_mutex_unlock(&player_database_mutex);
        savePlayerDatabase();
        return 0;
    }
}

// Updates a player's score.
void updatePlayerScore(const string& player_name, int attempt_score) {
    if (player_database.find(player_name) != player_database.end()) {
        pthread_mutex_lock(&player_database_mutex);
        player_database[player_name] += attempt_score;
        pthread_mutex_unlock(&player_database_mutex);
        savePlayerDatabase(); // Save the updated database to the file
        cout << "Added " << attempt_score << " to player '" << player_name << "'." << endl;
    } else {
        cout << "Player '" << player_name << "' not found in the database." << endl;
    }
}

// Comparison function for sorting pairs by score (in descending order)
bool compareByScore(const highscore_t& a, const highscore_t& b) {
    return a.score > b.score;
}

// Function to sort the player_database map and store the result in highscore array
void loadPlayerRanking() {
    // Create an array of highscore_t to store sorted data
    highscore_t highscore_from_playerbase[HIGHSCORE_SIZE];

    int i = 0;
    for (const auto& entry : player_database) {
        // Copy player data to the temporary array
        strncpy(highscore_from_playerbase[i].username, entry.first.c_str(), MAX_PLAYERNAME_SIZE);
        highscore_from_playerbase[i].score = entry.second;
        i++;
    }

    // Sort the highscore_t array in descending order of scores
    sort(highscore_from_playerbase, highscore_from_playerbase + HIGHSCORE_SIZE, compareByScore);

    // Ensure the highscore array is filled with default values first
    for (int j = 0; j < HIGHSCORE_SIZE; j++) {
        strncpy(highscore[j].username, "-----", MAX_PLAYERNAME_SIZE);
        highscore[j].score = 0;
    }

    // Copy the sorted data into the highscore array, up to the available size
    int copy_count = min(HIGHSCORE_SIZE, i);
    for (int j = 0; j < copy_count; j++) {
        strncpy(highscore[j].username, highscore_from_playerbase[j].username, MAX_PLAYERNAME_SIZE);
        highscore[j].score = highscore_from_playerbase[j].score;
    }
}


//Updates the player ranking
void updatePlayerRanking(const char player_name[MAX_PLAYERNAME_SIZE], int player_score) {
    pthread_mutex_lock(&highscore_mutex);
    
    // Check if the player is already in the highscore
    int player_position = -1;
    for (int i = 0; i < HIGHSCORE_SIZE; i++) {
        if (strcmp(highscore[i].username, player_name) == 0) {
            player_position = i;
            break;
        }
    }

    // If the player is in the highscore, update their score
    if (player_position != -1) {
        highscore[player_position].score = player_score;
    } else {
        // Find the position to insert the player
        int insert_position = HIGHSCORE_SIZE;
        while (insert_position > 0 && player_score > highscore[insert_position - 1].score) {
            insert_position--;
        }

        // Only update the ranking if the player's score is higher
        if (insert_position < HIGHSCORE_SIZE) {
            // Shift down the lower-ranked players to make room for the new player
            for (int i = HIGHSCORE_SIZE - 1; i > insert_position; i--) {
                strcpy(highscore[i].username, highscore[i - 1].username);
                highscore[i].score = highscore[i - 1].score;
            }

            // Insert the new player into the ranking
            strncpy(highscore[insert_position].username, player_name, MAX_PLAYERNAME_SIZE);
            highscore[insert_position].score = player_score;
        }
    }
    
    // Sort the ranking
    sort(highscore, highscore + HIGHSCORE_SIZE, compareByScore);
    
    pthread_mutex_unlock(&highscore_mutex);
}

#endif /* _PLAYER_DB_H_ */
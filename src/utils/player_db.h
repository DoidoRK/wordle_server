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
bool compareByScore(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

// Function to sort the player_database map and store the result in highscore array
void loadPlayerRanking() {
    // Create a vector of pairs (player name, score) from the map
    vector<pair<string, int>> player_vector;
    for (const auto& entry : player_database) {
        player_vector.push_back(entry);
    }

    // Sort the vector in descending order of scores
    sort(player_vector.begin(), player_vector.end(), compareByScore);

    // Copy the sorted data into the highscore array
    for (int i = 0; i < HIGHSCORE_SIZE; i++) {
        strncpy(highscore[i].username, player_vector[i].first.c_str(), MAX_PLAYERNAME_SIZE);
    }
}

//Updates the player ranking
void updatePlayerRanking(const char player_name[MAX_PLAYERNAME_SIZE], int player_score) {
     pthread_mutex_lock(&highscore_mutex);

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

    pthread_mutex_unlock(&highscore_mutex);
}

#endif /* _PLAYER_DB_H_ */
#ifndef _PLAYER_DB_H_
#define _PLAYER_DB_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <pthread.h>

#define PLAYER_BASE_PATH "db/players.txt"

using namespace std;

//Variable to keep track of player's scores during runtime.
map<string, int> player_database;

//Mutex to control threads access to playerbase db
pthread_mutex_t player_database_mutex = PTHREAD_MUTEX_INITIALIZER;

// Load the player database from the CSV file
void loadPlayerDatabase() {
    pthread_mutex_lock(&player_database_mutex);
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
    pthread_mutex_unlock(&player_database_mutex);
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

void updatePlayerScore(const string& player_name, int new_score) {
    if (player_database.find(player_name) != player_database.end()) {
        pthread_mutex_lock(&player_database_mutex);
        player_database[player_name] = new_score;
        pthread_mutex_unlock(&player_database_mutex);
        savePlayerDatabase(); // Save the updated database to the file
        cout << "Added " << new_score << " to player '" << player_name << "'." << endl;
    } else {
        cout << "Player '" << player_name << "' not found in the database." << endl;
    }
}

#endif /* _PLAYER_DB_H_ */
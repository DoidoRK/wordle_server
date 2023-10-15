#ifndef _PLAYER_DB_H_
#define _PLAYER_DB_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <mutex>

#define PLAYER_BASE_PATH "db/players.txt"

using namespace std;

map<string, int> playerDatabase;


//*************************************************
//*            PLAYER DATABASE FUNCTIONS          *
//*************************************************

// Load the player database from the CSV file
void loadPlayerDatabase() {

    ifstream file(PLAYER_BASE_PATH);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string playerName;
            int playerScore;
            if (getline(iss, playerName, ',') && (iss >> playerScore)) {
                playerDatabase[playerName] = playerScore;
            }
        }
        file.close();
    }
}

// Save the player database back to the CSV file
void savePlayerDatabase() {
    ofstream file(PLAYER_BASE_PATH);
    if (file.is_open()) {
        for (const auto& player : playerDatabase) {
            file << player.first << "," << player.second << endl;
        }
        file.close();
    }
}

// Check if a player is in the database, add with a score of 0 if not
int getPlayerScore(const string& playerName) {
    if (playerDatabase.find(playerName) != playerDatabase.end()) {
        // Player is in the database, return the score
        return playerDatabase[playerName];
    } else {
        // Player is not in the database, add with a score of 0
        playerDatabase[playerName] = 0;
        savePlayerDatabase();
        return 0;
    }
}

#endif /* _PLAYER_DB_H_ */
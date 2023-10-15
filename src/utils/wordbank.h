#ifndef _WORDBANK_H_
#define _WORDBANK_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>

//DB settings
#define WORDBANK_PATH "db/wordbank.txt"

using namespace std;

//Mutex to control threads access to playerbase
pthread_mutex_t wordbank_db_mutex = PTHREAD_MUTEX_INITIALIZER;
map<string, string> players_words;  //Stores the player name and the word drawn for the player.

bool searchWordInFile(const string& search_str) {
    pthread_mutex_lock(&wordbank_db_mutex);
    ifstream file(WORDBANK_PATH);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        pthread_mutex_unlock(&wordbank_db_mutex);
        return false;
    }
    string line;
    while (getline(file, line)) {
        if (line.find(search_str) != string::npos) {
            file.close();
            pthread_mutex_unlock(&wordbank_db_mutex);
            return true;
        }
    }
    file.close();
    pthread_mutex_unlock(&wordbank_db_mutex);
    return false;
}

string drawRandomWordFromFile() {
    pthread_mutex_lock(&wordbank_db_mutex);
    ifstream file(WORDBANK_PATH);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        pthread_mutex_unlock(&wordbank_db_mutex);
        return "";
    }

    vector<string> strings;
    string line;

    // Read each line from the file and store it in the vector
    while (getline(file, line)) {
        strings.push_back(line);
    }

    // Close the file
    file.close();

    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));

    if (strings.empty()) {
        cerr << "Error: The file is empty." << endl;
        pthread_mutex_unlock(&wordbank_db_mutex);
        return "";
    }

    // Generate a random index to select a string from the vector
    int random_index = rand() % strings.size();

    // Return the randomly selected string
    pthread_mutex_unlock(&wordbank_db_mutex);
    return strings[random_index];
}

bool isCharacterInWord(const char character, const string& word) {
    for (char c : word) {
        if (c == character) {
            return true;
        }
    }
    return false;
}

int* checkCharactersInWord(const string& guess, const string& word, int max_word_len) {
    int differences[WORD_SIZE];
    for (int i = 0; i < max_word_len; ++i) {
        if (guess[i] == word[i]) {
            differences[i] = 1;
        } else {
            if(isCharacterInWord(guess[i], word)){
                differences[i] = 2;
            }
        }
    }
    return differences;
}

void updatePlayersWords(const string& player_name){
    string new_word = drawRandomWordFromFile();
    cout << "Player:" << player_name << " Word Drawn: " << new_word << endl;
    pthread_mutex_lock(&wordbank_db_mutex);
    players_words[player_name] = new_word;
    pthread_mutex_unlock(&wordbank_db_mutex);
}

string getRightWordForPlayer(const string& player_name){
    if(players_words.find(player_name) != players_words.end()){
        return players_words[player_name];
    } else {
        return "";
    }
}

#endif /* _WORDBANK_H_ */
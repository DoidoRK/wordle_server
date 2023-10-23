#ifndef _WORDBANK_H_
#define _WORDBANK_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <pthread.h>
#include <cstring> // Added for string manipulation
#include <cctype>  // for toupper
#include <algorithm>  // for transform

#define WORDBANK_PATH "db/wordbank.txt"

using namespace std;

pthread_mutex_t wordbank_db_mutex = PTHREAD_MUTEX_INITIALIZER;
map<string, char[WORD_SIZE]> players_words; // Use 'string' for keys

string toUpperCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toupper(c);
    }
    return result;
}

string trim(const string& str) {
    // Find the first non-whitespace character
    size_t first = str.find_first_not_of(" \t\n\r");

    if (string::npos == first) {
        // No non-whitespace characters, return an empty string
        return str;
    }

    // Find the last non-whitespace character
    size_t last = str.find_last_not_of(" \t\n\r");

    // Return the trimmed string
    return str.substr(first, (last - first + 1));
}

bool searchWordInFile(const char word[WORD_SIZE]) {
    pthread_mutex_lock(&wordbank_db_mutex);
    ifstream file(WORDBANK_PATH);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return false;
    }

    string line;
    while (getline(file, line)){
        string trimmed_line = line;
        trimmed_line = toUpperCase(trimmed_line); // Ensure case-insensitive comparison
        trimmed_line = trim(trimmed_line);
        trimmed_line.erase(remove(trimmed_line.begin(), trimmed_line.end(), '\n'), trimmed_line.end());

        if (strcasecmp(trimmed_line.c_str(), word) == 0) {
            file.close();
            pthread_mutex_unlock(&wordbank_db_mutex);
            cout << "Word search finished, Word found: " << word << endl;
            return true;
        }
    }
    file.close();
    pthread_mutex_unlock(&wordbank_db_mutex);
    cout << "Word search finished, Word not found: " << word << endl;
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

    while (getline(file, line)) {
        strings.push_back(line);
    }

    file.close();

    srand(static_cast<unsigned>(time(nullptr)));

    if (strings.empty()) {
        cerr << "Error: The file is empty." << endl;
        pthread_mutex_unlock(&wordbank_db_mutex);
        return "";
    }

    int random_index = rand() % strings.size();

    pthread_mutex_unlock(&wordbank_db_mutex);
    return strings[random_index];
}

void updatePlayersWords(const char player_name[MAX_PLAYERNAME_SIZE], size_t word_size) {
    string new_word = drawRandomWordFromFile();
    pthread_mutex_lock(&wordbank_db_mutex);
    char new_word_cstr[WORD_SIZE];
    strncpy(new_word_cstr, new_word.c_str(), word_size);
    strncpy(players_words[player_name], new_word_cstr, word_size);
    pthread_mutex_unlock(&wordbank_db_mutex);
}

void getRightWordForPlayer(const string& player_name, char word[WORD_SIZE], size_t word_size) {
    auto it = players_words.find(player_name);
    if (it != players_words.end()) {
        const char* player_word = it->second;
        strncpy(word, player_word, word_size);
    } else {
        strncpy(word, "", word_size);
    }
}

#endif /* _WORDBANK_H_ */
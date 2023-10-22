#ifndef _WORDBANK_H_
#define _WORDBANK_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <pthread.h>
#include <cstring> // Added for string manipulation

#define WORDBANK_PATH "db/wordbank.txt"

using namespace std;

pthread_mutex_t wordbank_db_mutex = PTHREAD_MUTEX_INITIALIZER;
map<string, char[WORD_SIZE]> players_words; // Use 'string' for keys

#include <cctype>  // for std::toupper
#include <algorithm>  // for std::transform

// Convert a string to uppercase
std::string toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Trim leading and trailing whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool searchWordInFile(const char word[WORD_SIZE]) {
    pthread_mutex_lock(&wordbank_db_mutex);
    ifstream file(WORDBANK_PATH);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        pthread_mutex_unlock(&wordbank_db_mutex);
        return false;
    }

    char line[WORD_SIZE];
    while (file.getline(line, WORD_SIZE)) {
        // Trim any leading and trailing whitespace from the line
        string trimmedLine = line;
        trimmedLine = toUpperCase(trimmedLine); // Ensure case-insensitive comparison
        trimmedLine = trim(trimmedLine);

        if (strcasecmp(trimmedLine.c_str(), word) == 0) {
            file.close();
            pthread_mutex_unlock(&wordbank_db_mutex);
            cout << "ACHOU A PALAVRA!" << endl;
            return true;
        }
    }

    file.close();
    pthread_mutex_unlock(&wordbank_db_mutex);
    cout << "NÃO ACHOU A PALAVRA!" << endl;
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

bool isCharacterInWord(const char character, const string& word) {
    for (char c : word) {
        if (c == character) {
            return true;
        }
    }
    return false;
}

int* checkCharactersInWord(const string& guess, const string& word, int max_word_len) {
    static int differences[WORD_SIZE];
    for (int i = 0; i < max_word_len; ++i) {
        if (guess[i] == word[i]) {
            differences[i] = 1;
        } else {
            if (isCharacterInWord(guess[i], word)) {
                differences[i] = 2;
            }
        }
    }
    printf("Vai retornar as diferenças");
    return differences;
}

void updatePlayersWords(char player_name[MAX_PLAYERNAME_SIZE]) {
    string new_word = drawRandomWordFromFile();
    cout << "Player: " << player_name << " Word Drawn: " << new_word << endl;
    pthread_mutex_lock(&wordbank_db_mutex);

    char new_word_cstr[WORD_SIZE];
    strncpy(new_word_cstr, new_word.c_str(), WORD_SIZE - 1);
    new_word_cstr[WORD_SIZE - 1] = '\0';
    
    // Update the players_words map with the char array
    strncpy(players_words[player_name], new_word_cstr, WORD_SIZE);
    
    pthread_mutex_unlock(&wordbank_db_mutex);
}

void getRightWordForPlayer(const string& player_name, char word[WORD_SIZE], size_t word_size) {
    auto it = players_words.find(player_name);
    cout << "Jogador:" << player_name << endl;
    if (it != players_words.end()) {
        const char* player_word = it->second;
        strncpy(word, player_word, word_size - 1);
        word[word_size - 1] = '\0';
    } else {
        strncpy(word, "", word_size);
    }
}

#endif /* _WORDBANK_H_ */
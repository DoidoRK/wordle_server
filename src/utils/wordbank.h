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

//*************************************************
//*               WORDBANK FUNCTIONS              *
//*************************************************

bool searchStringInFile(const string& search_str) {
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

string drawRandomStringFromFile() {
    pthread_mutex_lock(&wordbank_db_mutex);
    ifstream file(WORDBANK_PATH);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
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
    pthread_mutex_unlock(&wordbank_db_mutex);

    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));

    if (strings.empty()) {
        cerr << "Error: The file is empty." << endl;
        return "";
    }

    // Generate a random index to select a string from the vector
    int random_index = rand() % strings.size();

    // Return the randomly selected string
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

vector<uint8_t> checkCharactersInWord(const string& guess, const string& word, int max_word_len) {
    vector<uint8_t> differences(max_word_len, 0);
    for (uint8_t i = 0; i < max_word_len; ++i) {
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

#endif /* _WORDBANK_H_ */
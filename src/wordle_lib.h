#ifndef _WORDLE_LIB_H_
#define _WORDLE_LIB_H_
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "utils.h"

#define MAX_WORD_LEN 5

using namespace std;

string drawRandomStringFromFile(const string& filename) {
    ifstream file(filename);
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

    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));

    if (strings.empty()) {
        cerr << "Error: The file is empty." << endl;
        return "";
    }

    // Generate a random index to select a string from the vector
    int randomIndex = rand() % strings.size();

    // Return the randomly selected string
    return strings[randomIndex];
}

vector<uint8_t> checkCharactersInWord(const string& guess, const string& word) {
    vector<uint8_t> differences(MAX_WORD_LEN, 0);
    for (uint8_t i = 0; i < MAX_WORD_LEN; ++i) {
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

#endif /* _WORDLE_LIB_H_ */
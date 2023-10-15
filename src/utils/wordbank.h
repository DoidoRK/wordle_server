#ifndef _WORDBANK_H_
#define _WORDBANK_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


//DB settings
#define WORDBANK_PATH "db/wordbank.txt"

using namespace std;



//*************************************************
//*               WORDBANK FUNCTIONS              *
//*************************************************

bool searchStringInFile(const string& searchStr) {
    ifstream file(WORDBANK_PATH);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        if (line.find(searchStr) != string::npos) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

string drawRandomStringFromFile() {
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

bool isCharacterInWord(const char character, const string& word) {
    for (char c : word) {
        if (c == character) {
            return true;
        }
    }
    return false;
}

vector<uint8_t> checkCharactersInWord(const string& guess, const string& word, int MAX_WORD_LEN) {
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

#endif /* _WORDBANK_H_ */
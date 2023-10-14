#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <iostream>
#include <inttypes.h>
#include <vector>
#include <string>

using namespace std;

vector<uint8_t> stringToUint8Vector(const string& str) {
    vector<uint8_t> result;
    for (char c : str) {
        result.push_back(static_cast<uint8_t>(c));
    }
    return result;
}

string uint8VectorToString(const vector<uint8_t>& uint8Vector) {
    string result;
    for (uint8_t val : uint8Vector) {
        result.push_back(static_cast<char>(val));
    }
    return result;
}

void printUint8Vector(const vector<uint8_t>& uint8Vector) {
    for (const uint8_t val : uint8Vector) {
        cout << static_cast<int>(val) << " ";
    }
    cout << endl;
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

#endif /* _STRING_UTILS_H_ */
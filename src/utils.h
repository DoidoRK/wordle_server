#ifndef _UTILS_H_
#define _UTILS_H_
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <fstream>
#include <string>
#include "config.h"

using namespace std;

int check(int exp, const char *msg) {
    if(exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return 0;
}

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

bool searchStringInFile(const string& filename, const string& searchStr) {
    ifstream file(filename);
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

bool isCharacterInWord(const char character, const string& word) {
    for (char c : word) {
        if (c == character) {
            return true;
        }
    }
    return false;
}

#endif /* _UTILS_H_ */
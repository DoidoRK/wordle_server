#ifndef _DB_UTLS_H_
#define _DB_UTLS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

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

#endif /* _DB_UTLS_H_ */
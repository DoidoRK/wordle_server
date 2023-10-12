#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

std::string drawRandomStringFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return "";
    }

    std::vector<std::string> strings;
    std::string line;

    // Read each line from the file and store it in the vector
    while (std::getline(file, line)) {
        strings.push_back(line);
    }

    // Close the file
    file.close();

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    if (strings.empty()) {
        std::cerr << "Error: The file is empty." << std::endl;
        return "";
    }

    // Generate a random index to select a string from the vector
    int randomIndex = std::rand() % strings.size();

    // Return the randomly selected string
    return strings[randomIndex];
}

bool searchStringInFile(const std::string& filename, const std::string& searchStr) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(searchStr) != std::string::npos) {
            std::cout << "Found: " << line << std::endl;
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}
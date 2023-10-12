#include <iostream>
#include "wordle_lib.h"
#include "utils.h"
#include<unistd.h>

#define WORD_BANK_FILENAME "src/db/wordbank.txt"

using namespace std;

int main() {
    string first_result = searchStringInFile(WORD_BANK_FILENAME, "hifen")? "Está no arquivo\n" : "Não está no arquivo\n";
    string second_result = searchStringInFile(WORD_BANK_FILENAME, "testi")? "Está no arquivo\n" : "Não está no arquivo\n";

    cout << first_result;
    cout << second_result;

    string randomString = drawRandomStringFromFile(WORD_BANK_FILENAME);
    if (!randomString.empty()) {
        cout << "Randomly drawn string: " << randomString << endl;
    } else {
        cout << "Failed to draw a random string." <<endl;
    }

    vector<uint8_t> uint8Vector = checkCharactersInWord("aeiou","teste"); //21110
    
    cout << "Uint8_t Vector: ";
    printUint8Vector(uint8Vector);

    return 0;
}
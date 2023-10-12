#ifndef _WORDLE_LIB_H_
#define _WORDLE_LIB_H_
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#define MAX_WORD_LEN 5

using namespace std;

bool searchStringInFile(const string& filename, const string& searchStr);

string drawRandomStringFromFile(const string& filename);

vector<uint8_t> checkCharactersInWord(const string& guess, const string& word);

#endif /* _WORDLE_LIB_H_ */
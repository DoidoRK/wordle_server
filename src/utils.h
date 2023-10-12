#ifndef _UTILS_H_
#define _UTILS_H_
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <fstream>
#include <string>

#define MAX_WORD_LEN 5

using namespace std;

vector<uint8_t> stringToUint8Vector(const string& str);

string uint8VectorToString(const vector<uint8_t>& uint8Vector);

void printUint8Vector(const vector<uint8_t>& uint8Vector);

bool searchStringInFile(const string& filename, const string& searchStr);

bool isCharacterInWord(const char character, const string& word);

#endif /* _UTILS_H_ */
#ifndef _WORDLE_UTILS_H_
#define _WORDLE_UTILS_H_
#include <iostream>

bool searchStringInFile(const std::string& filename, const std::string& searchStr);

std::string drawRandomStringFromFile(const std::string& filename);

#endif /* _WORDLE_UTILS_H_ */
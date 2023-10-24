#ifndef _RANKING_DB_H_
#define _RANKING_DB_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <pthread.h>
#include "../wordle_types.h"

#define RANKING_PATH "db/ranking.txt"

using namespace std;

pthread_mutex_t highscore_mutex = PTHREAD_MUTEX_INITIALIZER;
highscore_t highscore[HIGHSCORE_SIZE];

// Function to read the ranking from ranking.txt
void loadPlayerRanking(){
    pthread_mutex_lock(&highscore_mutex);
    ifstream rankingFile(RANKING_PATH);

    if (!rankingFile.is_open()) {
        cerr << "Error: Unable to open "<< RANKING_PATH << endl;
        pthread_mutex_unlock(&highscore_mutex);
    }
    string line;
    string username, score_string;
    int score, score_count = 0;

    while (getline(rankingFile,line))
    {
        stringstream ss(line);
        getline(ss, username,',');
        getline(ss,score_string);
        score = stoi(score_string);
        strncpy(highscore[score_count].username,username.c_str(),MAX_PLAYERNAME_SIZE);
        highscore[score_count].score = score;
        score_count++;
    }
    rankingFile.close();

    while(HIGHSCORE_SIZE > score_count){
        strcpy(highscore[score_count].username, "-----");
        highscore[score_count].score = 0;
        score_count++;
    }
    pthread_mutex_unlock(&highscore_mutex);
}

// void updatePlayerRanking(const char player_name[MAX_PLAYERNAME_SIZE], size_t word_size) {
//     string new_word = drawRandomWordFromFile();
//     pthread_mutex_lock(&highscore_mutex);
//     char new_word_cstr[WORD_SIZE];
//     strncpy(new_word_cstr, new_word.c_str(), word_size);
//     strncpy(players_words[player_name], new_word_cstr, word_size);
//     pthread_mutex_unlock(&highscore_mutex);
// }

#endif /* _RANKING_DB_H_ */
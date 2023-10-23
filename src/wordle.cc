#include <cstring>
#include "wordle.h"
#include "utils/player_db.h"
#include "utils/wordbank.h"
#include "utils/ranking_db.h"

const char* message_strings[] = {
    "INVALID_MESSAGE_TYPE",
    "INVALID_ATTEMPT_WORD",
    "PLAYER_NEW_WORD",
    "PLAYER_ATTEMPT",
    "GET_HIGHSCORE"
};

string  printMessage(long unsigned int message) {
    if (message >= 0 && message < sizeof(message_strings) / sizeof(message_strings[0])) {
        return message_strings[message];
    } else {
        return "INVALID_ENUM_VALUE";
    }
}

void initWordle(){
    loadPlayerDatabase();
}

void calculateAttemptAnswer(const char* attempt, const char* right_word, int* attempt_answer, size_t word_size) {
    for (size_t i = 0; i < word_size; i++) {
        if (attempt[i] == right_word[i]) {
            attempt_answer[i] = CHARACTER_IS_CORRECT;
        } else {
            for (size_t j = 0; j < word_size; j++) {
                if (attempt[i] == right_word[j] && attempt_answer[i] != CHARACTER_IS_CORRECT) {
                    attempt_answer[i] = CHARACTER_IS_IN_WORD;
                }
            }
        }
    }
}

data_packet_t playerNewWord(data_packet_t received_data){
    data_packet_t response;
    response.player.score = getPlayerScore(received_data.player.username);
    response.message_type = PLAYER_NEW_WORD;
    strncpy(response.player.username,received_data.player.username,MAX_PLAYERNAME_SIZE);
    updatePlayersWords(response.player.username);
    return response;
}


data_packet_t playerAttempt(data_packet_t received_data){
    data_packet_t response;
    const char* username = received_data.player.username;
    const char* attemptWord = received_data.player.current_attempt.word;
    const int attemptNumber = received_data.player.attempt_n;

    // Check if the player exceeded the maximum attempts
    if (attemptNumber >= MAX_ATTEMPTS) {
        cout << "Player " << username << " tried to have more attempts than allowed: " << attemptNumber << " attempts" << endl;
        response.message_type = INVALID_MESSAGE_TYPE;
        response.player.score = getPlayerScore(username);
        return response;
    }

    // Check if the player's word is valid
    if (!searchWordInFile(attemptWord)) {
        response.message_type = INVALID_ATTEMPT_WORD;
        response.player.score = getPlayerScore(username);
        return response;
    }

    char right_word[WORD_SIZE];
    getRightWordForPlayer(username, right_word, WORD_SIZE);

    if (right_word[0] == '\0') {
        cout << "Unknown Player " << username << " tried to attempt an invalid word: " << attemptWord << endl;
        response.message_type = INVALID_MESSAGE_TYPE;
        return response;
    }

    int attempt_answer[WORD_SIZE];
    int score_to_add = 0;
    calculateAttemptAnswer(attemptWord, right_word, attempt_answer, WORD_SIZE);
    for (int result : attempt_answer) {
        if (result == CHARACTER_IS_IN_WORD) {
            score_to_add += CHARACTER_IS_IN_WORD_SCORE;
        } else if (result == CHARACTER_IS_CORRECT) {
            score_to_add += CHARACTER_IS_CORRECT_SCORE;
        }
        cout << result << endl;
    }
    cout << "Player: "<< username << " score:" << score_to_add << endl;

    // Check if it's the last attempt
    if (attemptNumber == MAX_ATTEMPTS - 1) {
        response.message_type = PLAYER_NEW_WORD;
    } else if (score_to_add == (WORD_SIZE * CHARACTER_IS_CORRECT_SCORE)) {
        response.message_type = PLAYER_NEW_WORD;
    } else {
        response.message_type = PLAYER_ATTEMPT;
    }

    if ( 0 < score_to_add) {
        updatePlayerScore(username, score_to_add);
    }
    response.player.score = getPlayerScore(username);

    if (response.message_type == PLAYER_NEW_WORD) {
        updatePlayersWords(username);
    }

    return response;
}

data_packet_t threatMessage(data_packet_t received_data){
    data_packet_t response;
    cout << "Message:" << printMessage(received_data.message_type) << " from " << received_data.player.username << endl;
    switch (received_data.message_type)
    {        
        case PLAYER_NEW_WORD:
            // If the message type is this, it means that the player 
            // failed in his attempts and a new word must be draw.
            response = playerNewWord(received_data);
            break;

        case PLAYER_ATTEMPT:
            //Verifies if the player attempt is less than the maximun attempts.
            //Checks if the received word is the same as the word chosen for the player.
            
            response = playerAttempt(received_data);
            break;

        case GET_HIGHSCORE:
            //Returns the current highscore to the game client
            cout << "Get Highscore message received" << endl;
            break;


        default:
            cout << "Unknown message type received: " << received_data.message_type << endl;
            response.message_type = INVALID_MESSAGE_TYPE;
            break;
    }
    return response;
}
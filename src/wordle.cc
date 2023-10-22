#include <string.h>
#include "wordle.h"
#include "utils/player_db.h"
#include "utils/wordbank.h"
#include "utils/ranking_db.h"

const char* message_strings[] = {
    "INVALID_MESSAGE_TYPE",
    "PLAYER_JOINED",
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

data_packet_t playerJoined(data_packet_t received_data){
    data_packet_t response;
    response.player.score = getPlayerScore(received_data.player.username);
    response.message_type = PLAYER_NEW_WORD;
    strncpy(response.player.username,received_data.player.username,MAX_PLAYERNAME_SIZE);
    updatePlayersWords(response.player.username);
    return response;
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
    if(received_data.player.attempt_n > MAX_ATTEMPTS){
        if(searchWordInFile(received_data.player.current_attempt.word)){
            string right_word = getRightWordForPlayer(received_data.player.username);
            if(!right_word.empty()){
                int attempt_answer[WORD_SIZE];
                int score_to_add = 0;
                memcpy(attempt_answer, checkCharactersInWord(received_data.player.current_attempt.word, right_word, WORD_SIZE), WORD_SIZE);
                for (int result : attempt_answer)
                {
                    if(result == 2){
                        score_to_add += 2;
                    } else if(result == 1){
                        score_to_add += 5;
                    }
                }
                if (score_to_add)
                {
                    updatePlayerScore(received_data.player.username, score_to_add);
                    response.player.score = getPlayerScore(received_data.player.username);
                }
                memcpy(response.player.current_attempt.colors,attempt_answer,WORD_SIZE);
                response.message_type = PLAYER_ATTEMPT;
            } else {
                cout << "Unknown Player " << received_data.player.username << " tried to attempt an word" << endl;
                response.message_type = INVALID_MESSAGE_TYPE;
            }
        } else {
            response.message_type = INVALID_ATTEMPT_WORD;
        }
    } else {
        cout << "Player " << received_data.player.username << " tried to have more attempts than allowed: ";
        cout << received_data.player.attempt_n << " attempts" << endl;
        response.message_type = INVALID_MESSAGE_TYPE;
    }
    return response;
}

data_packet_t threatMessage(data_packet_t received_data){
    data_packet_t response;
    printf("Entrou no thread message");
    cout << "Message:" << printMessage(received_data.message_type) << endl;
    cout << "*******PLAYER INFO*******"<< endl;
    cout << "Player name: " << received_data.player.username << endl;
    switch (received_data.message_type)
    {
    case PLAYER_JOINED:
        response = playerJoined(received_data);
        break;
    
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
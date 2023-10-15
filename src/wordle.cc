#include <string.h>
#include "wordle.h"
#include "utils/player_db.h"
#include "utils/wordbank.h"
#include "utils/ranking_db.h"

const char* MESSAGE_STRINGS[] = {
    "PLAYER_JOINED",
    "PLAYER_NEW_WORD",
    "PLAYER_ATTEMPT",
    "GET_HIGHSCORE"
};

string  printMessage(long unsigned int message) {
    if (message >= 0 && message < sizeof(MESSAGE_STRINGS) / sizeof(MESSAGE_STRINGS[0])) {
        return MESSAGE_STRINGS[message];
    } else {
        return "INVALID_ENUM_VALUE";
    }
}

void initWordle(){
    loadPlayerDatabase();
}

data_packet_t playerJoined(data_packet_t received_data){
    //Check if player is in playerbase.
    //If it is in the playerbase:
    //  Sends back the player score;
    //  Draws a word for that player and starts a session;
    //If it isn't:
    //  Adds player to the DB;
    //  Draws a word for that player and starts a session;
    //Response type expected: PLAYER_NEW_WORD
    data_packet_t response;
    response.player.score = getPlayerScore(received_data.player.username);
    response.message_type = PLAYER_NEW_WORD;
    memcpy(response.player.username,received_data.player.username, sizeof(char[MAX_USERNAME_LEN]));
    return response;
}

data_packet_t threatMessage(data_packet_t received_data){
    data_packet_t response;
    cout << "Message:" << printMessage(received_data.message_type) << endl;
    cout << "*******PLAYER INFO*******"<< endl;
    cout << "Player name: " << received_data.player.username << endl;

    switch (received_data.message_type)
    {
    case PLAYER_JOINED:
        response = playerJoined(received_data);
        break;
    
    case PLAYER_NEW_WORD:
        //Draws a new word to the player.
        cout << "New word drawed to the player!" << endl;
        break;

    case PLAYER_ATTEMPT:
        //Verifies if the player attempt is less than the maximun attempts.
        //Checks if the received word is the same as the word chosen for the player.
        cout << "Player attempt received!" << endl;
        break;

    case GET_HIGHSCORE:
        //Returns the current highscore to the game client
        cout << "Get Highscore message received!" << endl;
        break;


    default:
        cout << "Unknown message type received!: " << received_data.message_type << endl;
        break;
    }


    return response;
}
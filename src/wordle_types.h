#ifndef _WORDLE_TYPES_H_
#define _WORDLE_TYPES_H_

//Wordle settings.
#define MAX_ATTEMPTS 6
#define MAX_USERNAME_LEN 32
#define WORD_SIZE 5
#define HIGHSCORE_PLAYER_NUM 5
#define HIGHSCORE_SIZE 5

enum MESSAGES {
    //If the player is a new player, adds the player to the player list with score 0.
    //If the player already exists in the DB, returns the player score.
    //Sorts a new word for that player.
    PLAYER_JOINED,

    //If the player failed to guess the right word, the server draws a new word for him.
    //If the player succeeded to guess the right word, the server adds to the player score and draws a new word
    PLAYER_NEW_WORD,

    //Checks if the word guessed by the player is correct and returns the result.
    PLAYER_ATTEMPT,

    //Gets the highscore to keep the player updated.
    GET_HIGHSCORE,
};

typedef struct attempt_struct{
    char word[WORD_SIZE];
    int colors[WORD_SIZE];
} attempt_t;

typedef struct user_struct{
    char username[MAX_USERNAME_LEN];
    int attempt_n;
    int score;
    attempt_t current_attempt;
} user_t;

typedef struct highscore_struct{
    char username[MAX_USERNAME_LEN];
    int score;
} highscore_t;

typedef struct data_packet_struct{
    int message_type;
    user_t player;
    highscore_t highscores[HIGHSCORE_SIZE];
} data_packet_t;

#endif /* _WORDLE_TYPES_H_ */
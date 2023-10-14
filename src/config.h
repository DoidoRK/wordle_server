#ifndef _CONFIG_H_
#define _CONFIG_H_

//DB settings
#define WORDBANK_PATH "db/wordbank.txt"

//Connection settings
#define SERVER_PORT 8080
#define MAX_CONNECTIONS 100

//Thread Pool Settings
#define THREAD_POOL_SIZE 20

//Wordle settings.
#define MAX_ATTEMPTS 6
#define USERNAME_LEN 64
#define WORD_SIZE 5
#define HIGHSCORE_PLAYER_NUM 5

typedef struct attempt_struct{
    uint8_t word[WORD_SIZE];
    uint8_t colors[WORD_SIZE];
} attempt_t;

typedef struct user_struct{
    uint8_t username[USERNAME_LEN];
    uint8_t attempt;
    uint16_t score;
    attempt_t attempts[MAX_ATTEMPTS];
} user_t;

typedef struct highscore_struct{
    uint8_t username[USERNAME_LEN];
    uint16_t score;
} highscore_t;

#endif /* _CONFIG_H_ */
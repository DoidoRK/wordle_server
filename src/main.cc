#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "wordle_lib.h"
#include "config.h"
#include "utils.h"

void * handleConnection(void* p_client_socket) {
    int client_socket = *((int*)p_client_socket);
    free(p_client_socket);
    int N=30;
    char buff[N];
    time_t ticks;
    ticks = time (NULL);
    snprintf (buff, sizeof (buff), "%.24s\r\n", ctime(&ticks));
    send(client_socket, &buff, sizeof(buff), 0);
    close(client_socket);
    return NULL;
}

pthread_t thread_pool[THREAD_POOL_SIZE];

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to open stream socket");

    /* Adresses */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    check((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0),"Failed to bind stream socket");
    
    /* Waits for connections */
    check((listen(server_socket,MAX_CONNECTIONS)), "Failed to listen");
    while (1) {
        client_socket = accept(server_socket,(struct sockaddr *)0,0);
        pthread_t t;
        int *pclient = (int*)malloc(sizeof(int));
        *pclient = client_socket;
        pthread_create(&t,NULL, handleConnection, pclient);
    }
    return 0;
}
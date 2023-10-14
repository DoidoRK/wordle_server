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
#include "queue.h"
#include "config.h"
#include "utils.h"

pthread_t thread_pool[THREAD_POOL_SIZE];
int connection_queue_count = 0;
queue_t connection_queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

void * handleConnection(void *p_client_socket){
    int client_socket = *((int*)p_client_socket);
    free((int*)p_client_socket);
    int N=30;
    char buff[N];
    time_t ticks;
    ticks = time (NULL);
    snprintf (buff, sizeof (buff), "%.24s\r\n", ctime(&ticks));
    send(client_socket, &buff, sizeof(buff), 0);
    close(client_socket);
    return NULL;
};

void * connectionThread(void* arg) {
    while (true)
    {
        pthread_mutex_lock(&queue_mutex);
        int *p_client_socket = (int*)dequeue(&connection_queue);
        if(NULL != p_client_socket){
            //New connection received.
            printf("Connection received!\n");
            handleConnection(p_client_socket);
        }
        pthread_mutex_unlock(&queue_mutex);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;

    init_queue(&connection_queue);

    //Creates server threads to handle future connections.
    for (uint8_t i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&thread_pool[i],NULL, connectionThread, NULL);
    }
    
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to open stream socket");

    /* Adresses */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    check((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0),"Failed to bind stream socket");
    
    /* Waits for connections */
    check((listen(server_socket,MAX_CONNECTIONS)), "Failed to listen");
    if(-1 == printServerMessage()){
        cout << "Failed to get server IP address!" << endl;
    };
    while (1) {
        check((client_socket = accept(server_socket,(struct sockaddr *)0,0)),"Accept Failed");
        //Sends connection to Queue
        int *pclient = (int*)malloc(sizeof(int));
        *pclient = client_socket;
        pthread_mutex_lock(&queue_mutex);
        enqueue(&connection_queue,pclient);
        pthread_mutex_unlock(&queue_mutex);
    }
    return 0;
}
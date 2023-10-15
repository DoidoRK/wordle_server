#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include "config.h"
#include "wordle.h"
#include "utils/network_utils.h"
#include "libs/queue.h"
#include "libs/conio_linux.h"

//Thread pool to reduce server memory usage.
pthread_t thread_pool[THREAD_POOL_SIZE];

//Queue to store received connections and threat them as the threads get ready.
queue_t connection_queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

//Conditional variable to wake up threads when work is needed to be done.
pthread_cond_t new_connection_arrived = PTHREAD_COND_INITIALIZER;

void * handleConnection(void *p_client_socket){
    int client_socket = *((int*)p_client_socket);
    free((int*)p_client_socket);
    data_packet_t message, response;

    recv(client_socket, &message, sizeof(data_packet_t), 0);

    response = threatMessage(message);

    send(client_socket, &response, sizeof(data_packet_t), 0);

    close(client_socket);
    return NULL;
};

void * connectionThread(void* arg) {
    while (true)
    {
        int *p_client_socket;
        pthread_mutex_lock(&queue_mutex);
        if(NULL == (p_client_socket = (int*)dequeue(&connection_queue))){
            //The threads will only wait for the signal if there is no work on the queue
            pthread_cond_wait(&new_connection_arrived, &queue_mutex);
            p_client_socket = (int*)dequeue(&connection_queue);
        }
        if(NULL != p_client_socket){
            //New connection received.
            handleConnection(p_client_socket);
        }
        pthread_mutex_unlock(&queue_mutex);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;

    clrscr();

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
    if(-1 == serverMessage(to_string(SERVER_PORT))){
        cout << "Failed to get server IP address!" << endl;
    };
    while (1) {
        check((client_socket = accept(server_socket,(struct sockaddr *)0,0)),"Accept Failed");
        int *pclient = (int*)malloc(sizeof(int));
        *pclient = client_socket;
        pthread_mutex_lock(&queue_mutex);
        enqueue(&connection_queue,pclient); //Sends connection to Queue
        pthread_cond_signal(&new_connection_arrived);
        pthread_mutex_unlock(&queue_mutex);
    }
    return 0;
}
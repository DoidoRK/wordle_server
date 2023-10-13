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



int main() {
    int sock, length, rval, s0, N=30;
    struct sockaddr_in server;
    time_t ticks;
    char buff[N];
    sock = socket(AF_INET, SOCK_STREAM, 0); /* Create socket. */
    if (sock < 0) {
        perror("opening stream socket");
        exit(0);
    }

    /* Adresses */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    length = sizeof(server);
    if (bind(sock, (struct sockaddr *)&server, length) < 0){
        perror("binding stream socket");
        exit(0);
    }
    
    /* Waits for connections */
    listen(sock,5);
    
    for (;;) {
        printf("Servidor: Aguardando conexao!\n");
        s0 = accept(sock,(struct sockaddr *)0,0);
        ticks = time (NULL);
        snprintf (buff, sizeof (buff), "%.24s\r\n", ctime(&ticks));
        printf("Servidor: vou enviar a hora!\n");
        rval = send(s0, &buff, sizeof(buff), 0);
        printf("Servidor: enviei a hora!\n");
    }
    close(s0); //close(sock);
    return 0;
}
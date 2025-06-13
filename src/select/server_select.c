#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define PORTA 8080
#define MAX_CLIENTES FD_SETSIZE

int main() {
    int listener_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    struct sockaddr_in cli_addrs[MAX_CLIENTES];
    fd_set master_set, read_fds;
    int fdmax;

    listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_socket < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORTA);

    if (bind(listener_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro no bind");
        close(listener_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(listener_socket, 10) < 0) {
        perror("Erro no listen");
        close(listener_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor web com select rodando na porta %d...\n", PORTA);

    FD_ZERO(&master_set);
    FD_SET(listener_socket, &master_set);
    fdmax = listener_socket;

    while (1) {
        read_fds = master_set;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Erro no select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener_socket) {
                    client_socket = accept(listener_socket, (struct sockaddr *)&client_addr, &client_len);
                    if (client_socket < 0) {
                        perror("Erro no accept");
                        continue;
                    }

                    printf("Nova conexão de %s\n", inet_ntoa(client_addr.sin_addr));

                    FD_SET(client_socket, &master_set);
                    if (client_socket > fdmax) fdmax = client_socket;

                    cli_addrs[client_socket] = client_addr; 
                } else {
                    tratar_conexao(i, &cli_addrs[i]);
                    close(i);
                    FD_CLR(i, &master_set);
                }
            }
        }
    }

    close(listener_socket);
    return 0;
}

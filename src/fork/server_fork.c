#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORTA 8080

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORTA);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro no bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Erro no listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor web com fork rodando na porta %d...\n", PORTA);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Erro no accept");
            continue;
        }

        printf("Nova conexão de %s\n", inet_ntoa(client_addr.sin_addr));

        pid_t pid = fork();
        if (pid == 0) {
            close(server_socket); 
            tratar_conexao(client_socket, &client_addr);
            close(client_socket);
            exit(0);
        } else if (pid > 0) {
            close(client_socket); 
        } else {
            perror("Erro no fork");
            close(client_socket);
        }

        while (waitpid(-1, NULL, WNOHANG) > 0);
    }

    close(server_socket);
    return 0;
}

#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define PORTA 8080
#define MAX_CLIENTES 1000  
#define N_THREADS 32      

typedef struct {
    int client_socket;
    struct sockaddr_in cli_addr;
} tarefa_socket_t;

tarefa_socket_t fila_sockets[MAX_CLIENTES];
int pos_in = 0;
int pos_out = 0;
int fila_size = 0;  

pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_fila;

void inserir_fila(tarefa_socket_t tarefa) {
    pthread_mutex_lock(&mutex_fila);

    if (fila_size >= MAX_CLIENTES) {
        pthread_mutex_unlock(&mutex_fila);
        printf("Fila cheia! Descartando conexão de %s\n", inet_ntoa(tarefa.cli_addr.sin_addr));
        close(tarefa.client_socket);
        return;
    }

    fila_sockets[pos_in] = tarefa;
    pos_in = (pos_in + 1) % MAX_CLIENTES;
    fila_size++;

    pthread_mutex_unlock(&mutex_fila);
    sem_post(&sem_fila);
}

tarefa_socket_t remover_fila() {
    sem_wait(&sem_fila);
    pthread_mutex_lock(&mutex_fila);

    tarefa_socket_t tarefa = fila_sockets[pos_out];
    pos_out = (pos_out + 1) % MAX_CLIENTES;
    fila_size--;

    pthread_mutex_unlock(&mutex_fila);
    return tarefa;
}

void *thread_worker(void *) {
    while (1) {
        tarefa_socket_t tarefa = remover_fila();
        tratar_conexao(tarefa.client_socket, &tarefa.cli_addr);
        close(tarefa.client_socket);
    }
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    signal(SIGPIPE, SIG_IGN);

    sem_init(&sem_fila, 0, 0);

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

    printf("Servidor web com threads + fila rodando na porta %d...\n", PORTA);

    pthread_t threads[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_worker, NULL);
    }

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Erro no accept");
            continue;
        }

        printf("Nova conexão de %s\n", inet_ntoa(client_addr.sin_addr));

        tarefa_socket_t tarefa;
        tarefa.client_socket = client_socket;
        tarefa.cli_addr = client_addr;

        inserir_fila(tarefa); 
    }

    close(server_socket);
    return 0;
}

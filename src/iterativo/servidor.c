#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

void log_requisicao(struct sockaddr_in *cli_addr, const char *request_line) {
    time_t agora = time(NULL);
    char *timestamp = ctime(&agora);
    timestamp[strcspn(timestamp, "\n")] = '\0';

    printf("[LOG] IP: %s | Hora: %s | Requisicao: %s\n",
           inet_ntoa(cli_addr->sin_addr),
           timestamp,
           request_line);
}

void tratar_conexao(int client_socket, struct sockaddr_in *cli_addr) {
    char buffer[BUFFER_SIZE];
    int bytes_recebidos;

    bytes_recebidos = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_recebidos == 0) {
        printf("Conexao fechada pelo cliente.\n");
        return;
    } else if (bytes_recebidos < 0) {
        perror("Erro ao receber dados");
        return;
    }

    buffer[bytes_recebidos] = '\0';

    char metodo[16], caminho[256], protocolo[16];
    sscanf(buffer, "%s %s %s", metodo, caminho, protocolo);

    log_requisicao(cli_addr, buffer);

    char caminho_arquivo[512];
    if (strcmp(caminho, "/") == 0) {
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/testeindex.html", ROOT);
    } else {
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s%s", ROOT, caminho);
    }

    FILE *arquivo = fopen(caminho_arquivo, "rb");
    if (arquivo == NULL) {
        const char *resposta_404 =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n";
        send(client_socket, resposta_404, strlen(resposta_404), 0);
        return;
    }

    const char *content_type = "text/plain";
    if (strstr(caminho_arquivo, ".html")) content_type = "text/html";
    else if (strstr(caminho_arquivo, ".jpg")) content_type = "image/jpeg";
    else if (strstr(caminho_arquivo, ".png")) content_type = "image/png";
    else if (strstr(caminho_arquivo, ".pdf")) content_type = "application/pdf";

    const char *cabecalho_fmt =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Connection: close\r\n"
        "\r\n";

    char cabecalho[BUFFER_SIZE];
    snprintf(cabecalho, sizeof(cabecalho), cabecalho_fmt, content_type);
    send(client_socket, cabecalho, strlen(cabecalho), 0);

    char file_buffer[BUFFER_SIZE];
    int n;
    while ((n = fread(file_buffer, 1, BUFFER_SIZE, arquivo)) > 0) {
        send(client_socket, file_buffer, n, 0);
    }

    fclose(arquivo);
}

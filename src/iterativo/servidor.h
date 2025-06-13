#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <netinet/in.h> 

#define BUFFER_SIZE 4096
#define ROOT "../arquivos"

void tratar_conexao(int client_socket, struct sockaddr_in *cli_addr);

#endif

# tp2_redes
Implementação de um servidor web, utilizando quatro técnicas distintas de programação com sockets para análises comparativas.

===================================================================
Trabalho Prático 2 - Redes de Computadores
Servidor Web com diferentes abordagens de sockets
===================================================================

Autor: Lucas Mendonça Sacchi
Data: Junho 2025

===================================================================
Descrição do Projeto
===================================================================

Este projeto implementa um servidor web simples em C utilizando
quatro abordagens diferentes de programação com sockets:

1) Servidor Iterativo (sequencial)
2) Servidor com Fork (um processo por conexão)
3) Servidor com Threads e Fila (pool de threads + fila circular)
4) Servidor com Select (I/O multiplexado)

As abordagens permitem comparar desempenho, escalabilidade e
robustez em cenários com múltiplas conexões.

Os servidores retornam arquivos estáticos (HTML, PNG, PDF, etc.)
para requisições HTTP GET feitas por navegadores ou ferramentas
de benchmark.

===================================================================
Estrutura do Projeto
===================================================================

tp2_redes/
├── src/
│   ├── iterativo/            --> versão server_iterativo
│   ├── fork/                 --> versão server_fork
│   ├── threads/              --> versão server_threads
│   ├── select/               --> versão server_select
│   ├── servidor.c / servidor.h --> funções comuns (tratar_conexao, log_requisicao, etc)
│   └── arquivos/             --> arquivos servidos (testeindex.html, testefoto.png, testepdf.pdf)
├── scripts/
│   ├── script_testes_ab.sh   --> script para rodar testes automatizados
├── README.txt                --> este arquivo

===================================================================
Como Compilar e Rodar o Código Manualmente
===================================================================

1) Abra um terminal e navegue até a pasta da versão que você deseja testar, por exemplo:

cd src/iterativo
ou
cd src/fork
ou
cd src/threads
ou
cd src/select

2) Compile o servidor com:

make

3) Execute o servidor:

make run

4) Acesse o servidor abrindo seu navegador e digitando:

http://localhost:8080

Você pode testar acessando diretamente:

- http://localhost:8080/testeindex.html
- http://localhost:8080/testepdf.pdf
- http://localhost:8080/testefoto.png

===================================================================
Como Rodar os Testes Automatizados com AB
===================================================================

Pré-requisitos:

- Instalar o Apache Benchmark (ab), em sistemas Linux:

sudo apt install apache2-utils

===================================================================
Passos:

1) Abra o terminal.

2) Navegue até a pasta da versão que você deseja testar:

cd src/iterativo
cd src/fork
cd src/threads
cd src/select

3) Compile o servidor:

make

4) Rode o servidor em um terminal (deixe ele rodando):

make run

5) Em outro terminal, vá para a pasta da versão que você deseja testar:

cd src/<versão>

7) Torne o script executável (se necessário):

chmod +x <nome_do_script>.sh

8) Execute o script:

./<nome_do_script>.sh

O script irá rodar automaticamente os testes com várias quantidades de conexões concorrentes e gera um arquivo:

- ab_results_<versao>.txt

Exemplo:

- ab_results_iterativo.txt
- ab_results_fork.txt
- ab_results_threads.txt
- ab_results_select.txt

===================================================================
Observações Finais
===================================================================

- O servidor deve estar rodando antes de executar o script de testes AB.
- Durante os testes você verá o log das requisições no terminal.
- Todas as versões foram projetadas para rodar na porta 8080.
- Não foram observadas perdas de requisições em nenhuma versão durante os testes.

===================================================================
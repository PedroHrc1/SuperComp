#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;
    char send_message[50];
    char recv_message[50];
    int send_dest, recv_source;

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verifica se há pelo menos 3 processos
    if (size < 3) {
        if (rank == 0) {
            printf("Este programa requer pelo menos 3 processos.\n");
            fflush(stdout);
        }
        MPI_Finalize();
        exit(0);
    }

    // Define o destino e a fonte para cada processo
    send_dest = (rank + 1) % size;
    recv_source = (rank - 1 + size) % size;

    // Prepara a mensagem a ser enviada
    snprintf(send_message, sizeof(send_message), "Mensagem do Processo %d", rank);

    // Envia e recebe mensagens de forma sincronizada para evitar deadlocks
    MPI_Sendrecv(
        send_message,            // Buffer de envio
        strlen(send_message) + 1,// Número de elementos a enviar (+1 para o caractere nulo)
        MPI_CHAR,                // Tipo de dados
        send_dest,               // Destino
        0,                       // Tag
        recv_message,            // Buffer de recebimento
        sizeof(recv_message),    // Número máximo de elementos a receber
        MPI_CHAR,                // Tipo de dados
        recv_source,             // Fonte
        0,                       // Tag
        MPI_COMM_WORLD,          // Comunicador
        &status                  // Status
    );

    // Exibe a mensagem recebida
    printf("Processo %d recebeu: '%s' do Processo %d\n", rank, recv_message, recv_source);
    fflush(stdout);

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

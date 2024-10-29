#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;
    char message[20];

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);
    
    // Obtém o rank do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Verifica se há pelo menos 2 processos
    if (size < 2) {
        if (rank == 0) {
            printf("Este programa requer pelo menos 2 processos.\n");
            fflush(stdout);
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        // Processo de rank 0 envia "Olá" para o processo de rank 1
        strcpy(message, "Olá");
        MPI_Send(message, strlen(message)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        printf("Processo 0 enviou: %s para o Processo 1\n", message);
        fflush(stdout);
        
        // Processo de rank 0 recebe "Oi" do processo de rank 1
        MPI_Recv(message, sizeof(message), MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        printf("Processo 0 recebeu: %s do Processo 1\n", message);
        fflush(stdout);
    }
    else if (rank == 1) {
        // Processo de rank 1 recebe "Olá" do processo de rank 0
        MPI_Recv(message, sizeof(message), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        printf("Processo 1 recebeu: %s do Processo 0\n", message);
        fflush(stdout);
        
        // Processo de rank 1 envia "Oi" de volta para o processo de rank 0
        strcpy(message, "Oi");
        MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("Processo 1 enviou: %s para o Processo 0\n", message);
        fflush(stdout);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

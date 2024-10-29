#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE_TAG 0
#define MESSAGE_SIZE 256  // Aumentado para evitar truncamento

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;
    char message[MESSAGE_SIZE];
    int mpi_error;

    // Inicializa o ambiente MPI
    mpi_error = MPI_Init(&argc, &argv);
    if (mpi_error != MPI_SUCCESS) {
        fprintf(stderr, "Erro ao inicializar MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, mpi_error);
    }

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
        exit(0);
    }

    if (rank == 0) {
        // Processo 0: envia a mensagem inicial para o Processo 1
        strncpy(message, "Olá, este é o processo 0 iniciando a difusão!", MESSAGE_SIZE - 1);
        message[MESSAGE_SIZE - 1] = '\0';  // Garantir terminação nula
        mpi_error = MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, MESSAGE_TAG, MPI_COMM_WORLD);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao enviar mensagem para o Processo 1.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d enviou: '%s' para o Processo %d\n", rank, message, 1);
        fflush(stdout);
    }
    else if (rank < size - 1) {
        // Processos intermediários: recebem do processo anterior e enviam para o próximo
        mpi_error = MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, rank - 1, MESSAGE_TAG, MPI_COMM_WORLD, &status);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao receber mensagem do Processo %d.\n", rank, rank - 1);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d recebeu: '%s' do Processo %d\n", rank, message, rank - 1);
        fflush(stdout);

        // Modifica a mensagem de forma segura
        char modification[50];
        snprintf(modification, sizeof(modification), " [Modificada pelo processo %d]", rank);
        strncat(message, modification, MESSAGE_SIZE - strlen(message) - 1);

        // Envia para o próximo processo
        mpi_error = MPI_Send(message, strlen(message) + 1, MPI_CHAR, rank + 1, MESSAGE_TAG, MPI_COMM_WORLD);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao enviar mensagem para o Processo %d.\n", rank, rank + 1);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d enviou: '%s' para o Processo %d\n", rank, message, rank + 1);
        fflush(stdout);
    }
    else if (rank == size - 1) {
        // Último processo: recebe do processo anterior e exibe a mensagem
        mpi_error = MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, rank - 1, MESSAGE_TAG, MPI_COMM_WORLD, &status);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao receber mensagem do Processo %d.\n", rank, rank - 1);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d recebeu: '%s' do Processo %d\n", rank, message, rank - 1);
        fflush(stdout);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

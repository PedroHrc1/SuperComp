#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE_TAG 0
#define MESSAGE_SIZE 100

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
        // Processo 0: envia mensagens exclusivas para todos os outros processos
        for (int dest = 1; dest < size; dest++) {
            // Prepara a mensagem exclusiva
            snprintf(message, sizeof(message), "Mensagem para o processo %d", dest);

            // Envia a mensagem para o processo de destino
            mpi_error = MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, MESSAGE_TAG, MPI_COMM_WORLD);
            if (mpi_error != MPI_SUCCESS) {
                fprintf(stderr, "Processo %d: Erro ao enviar mensagem para o Processo %d.\n", rank, dest);
                MPI_Abort(MPI_COMM_WORLD, mpi_error);
            }

            printf("Processo %d enviou: '%s' para o Processo %d\n", rank, message, dest);
            fflush(stdout);
        }
    }
    else {
        // Processos de rank > 0: recebem mensagem do processo 0
        mpi_error = MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 0, MESSAGE_TAG, MPI_COMM_WORLD, &status);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao receber mensagem do Processo 0.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }

        // Exibe a mensagem recebida
        printf("Processo %d recebeu: '%s' do Processo 0\n", rank, message);
        fflush(stdout);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAG_MESSAGE 0
#define TAG_RESPONSE 1
#define MESSAGE_SIZE 100

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;
    char message[MESSAGE_SIZE];
    char response[MESSAGE_SIZE];
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
        // Processo 0: envia mensagens para cada processo sequencialmente
        for (int dest = 1; dest < size; dest++) {
            // Prepara a mensagem
            snprintf(message, sizeof(message), "Olá, Processo %d! Esta é a mensagem %d do Processo 0.", dest, dest);

            // Envia a mensagem para o processo de destino
            mpi_error = MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, TAG_MESSAGE, MPI_COMM_WORLD);
            if (mpi_error != MPI_SUCCESS) {
                fprintf(stderr, "Processo %d: Erro ao enviar mensagem para o Processo %d.\n", rank, dest);
                MPI_Abort(MPI_COMM_WORLD, mpi_error);
            }
            printf("Processo %d enviou para o Processo %d: '%s'\n", rank, dest, message);
            fflush(stdout);

            // Aguarda a resposta do processo de destino
            mpi_error = MPI_Recv(response, MESSAGE_SIZE, MPI_CHAR, dest, TAG_RESPONSE, MPI_COMM_WORLD, &status);
            if (mpi_error != MPI_SUCCESS) {
                fprintf(stderr, "Processo %d: Erro ao receber resposta do Processo %d.\n", rank, dest);
                MPI_Abort(MPI_COMM_WORLD, mpi_error);
            }
            printf("Processo %d recebeu do Processo %d: '%s'\n", rank, dest, response);
            fflush(stdout);
        }
    }
    else {
        // Processos de rank > 0: recebem mensagem do Processo 0 e respondem
        // Recebe a mensagem do Processo 0
        mpi_error = MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 0, TAG_MESSAGE, MPI_COMM_WORLD, &status);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao receber mensagem do Processo %d.\n", rank, 0);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d recebeu do Processo %d: '%s'\n", rank, 0, message);
        fflush(stdout);

        // Prepara a resposta
        snprintf(response, sizeof(response), "Olá, Processo 0! Recebi sua mensagem %d.", rank);

        // Envia a resposta de volta para o Processo 0
        mpi_error = MPI_Send(response, strlen(response) + 1, MPI_CHAR, 0, TAG_RESPONSE, MPI_COMM_WORLD);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao enviar resposta para o Processo %d.\n", rank, 0);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d enviou para o Processo %d: '%s'\n", rank, 0, response);
        fflush(stdout);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

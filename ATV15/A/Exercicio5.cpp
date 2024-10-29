#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAG_SUM 0

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;
    int value;       // Valor exclusivo de cada processo
    int sum = 0;     // Soma acumulada

    // Inicializa o ambiente MPI
    int mpi_error = MPI_Init(&argc, &argv);
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

    // Atribui um valor exclusivo a cada processo
    // Por exemplo, o valor pode ser igual ao rank + 1
    value = rank + 1;

    if (rank == 0) {
        // Processo 0: inicia a soma e envia para o processo 1
        sum = value;
        printf("Processo %d inicia a soma com valor: %d\n", rank, sum);
        fflush(stdout);

        // Envia a soma atualizada para o próximo processo
        mpi_error = MPI_Send(&sum, 1, MPI_INT, rank + 1, TAG_SUM, MPI_COMM_WORLD);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao enviar soma para o Processo %d.\n", rank, rank + 1);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d enviou a soma %d para o Processo %d\n", rank, sum, rank + 1);
        fflush(stdout);
    }
    else {
        // Processos de rank 1 até size-1
        // Recebem a soma do processo anterior
        mpi_error = MPI_Recv(&sum, 1, MPI_INT, rank - 1, TAG_SUM, MPI_COMM_WORLD, &status);
        if (mpi_error != MPI_SUCCESS) {
            fprintf(stderr, "Processo %d: Erro ao receber soma do Processo %d.\n", rank, rank - 1);
            MPI_Abort(MPI_COMM_WORLD, mpi_error);
        }
        printf("Processo %d recebeu a soma %d do Processo %d\n", rank, sum, rank - 1);
        fflush(stdout);

        // Adiciona seu valor exclusivo à soma
        sum += value;
        printf("Processo %d adicionou seu valor %d. Soma atualizada: %d\n", rank, value, sum);
        fflush(stdout);

        if (rank != size - 1) {
            // Envia a soma atualizada para o próximo processo
            mpi_error = MPI_Send(&sum, 1, MPI_INT, rank + 1, TAG_SUM, MPI_COMM_WORLD);
            if (mpi_error != MPI_SUCCESS) {
                fprintf(stderr, "Processo %d: Erro ao enviar soma para o Processo %d.\n", rank, rank + 1);
                MPI_Abort(MPI_COMM_WORLD, mpi_error);
            }
            printf("Processo %d enviou a soma %d para o Processo %d\n", rank, sum, rank + 1);
            fflush(stdout);
        }
        else {
            // Último processo: exibe a soma total
            printf("Processo %d é o último processo. Soma total: %d\n", rank, sum);
            fflush(stdout);
        }
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

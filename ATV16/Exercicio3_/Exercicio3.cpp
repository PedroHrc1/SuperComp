#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h> // Necessário para srand() e rand()
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char *argv[]) {
    int rank, size;
    long long int N = 10; // Defina aqui o tamanho do vetor
    double *vector = NULL;
    double *local_vector = NULL;
    double local_sum = 0.0, total_sum = 0.0;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cálculo do tamanho local para cada processo
    long long int base_N = N / size;
    long long int remainder = N % size;
    long long int local_N = base_N;

    // Ajuste para distribuir o restante entre os processos iniciais
    if (rank < remainder) {
        local_N += 1;
    }

    // Inicialização do vetor no rank 0
    if (rank == 0) {
        vector = (double *)malloc(N * sizeof(double));
        if (vector == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o vetor.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        // Inicialização com valores aleatórios
        srand(time(NULL)); // Inicializa o gerador de números aleatórios
        for (long long int i = 0; i < N; i++) {
            vector[i] = (double)(rand() % 100); // Valores entre 0 e 99
        }
    }

    // Alocar o vetor local
    local_vector = (double *)malloc(local_N * sizeof(double));
    if (local_vector == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor local no processo %d.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Distribuição do vetor usando MPI_Scatterv
    int *sendcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        sendcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));
        long long int offset = 0;
        for (int i = 0; i < size; i++) {
            long long int count = base_N;
            if (i < remainder) {
                count += 1;
            }
            sendcounts[i] = count * sizeof(double);
            displs[i] = offset * sizeof(double);
            offset += count;
        }
    }

    // Ajuste do recvcount para MPI_Scatterv
    int recvcount = local_N * sizeof(double);

    MPI_Scatterv(vector, sendcounts, displs, MPI_BYTE,
                 local_vector, recvcount, MPI_BYTE,
                 0, MPI_COMM_WORLD);

    // Cálculo da soma parcial com OpenMP
    #pragma omp parallel for reduction(+:local_sum)
    for (long long int i = 0; i < local_N; i++) {
        local_sum += local_vector[i];
    }

    // Liberação da memória do vetor local
    free(local_vector);
    if (rank == 0) {
        free(vector);
        free(sendcounts);
        free(displs);
    }

    // Redução das somas parciais no processo 0
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Cálculo da média no processo 0
    if (rank == 0) {
        double media = total_sum / N;
        printf("A média dos elementos do vetor é: %f\n", media);
    }

    MPI_Finalize();
    return 0;
}

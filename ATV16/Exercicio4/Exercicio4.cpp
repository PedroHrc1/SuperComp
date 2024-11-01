#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char *argv[]) {
    int rank, size;
    long long int N = 100; // Defina aqui o tamanho do vetor
    int valor_buscado = 42; // Defina aqui o valor a ser buscado
    int *vector = NULL;
    int *local_vector = NULL;
    long long int base_N, remainder, local_N;
    long long int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cálculo do tamanho local para cada processo
    base_N = N / size;
    remainder = N % size;
    local_N = base_N;
    if (rank < remainder) {
        local_N += 1;
    }

    // Inicialização do vetor no rank 0
    if (rank == 0) {
        vector = (int *)malloc(N * sizeof(int));
        if (vector == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o vetor.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        // Inicialização com valores aleatórios entre 0 e 99
        srand(42); // Usando uma semente fixa para reproduzir os resultados
        for (long long int i = 0; i < N; i++) {
            vector[i] = rand() % 100;
        }
        // Opcional: inserir o valor buscado em posições conhecidas para teste
        vector[N / 2] = valor_buscado;
        vector[N - 1] = valor_buscado;
    }

    // Preparação para MPI_Scatterv
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
            sendcounts[i] = count * sizeof(int);
            displs[i] = offset * sizeof(int);
            offset += count;
        }
    }

    // Alocar o vetor local
    local_vector = (int *)malloc(local_N * sizeof(int));
    if (local_vector == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor local no processo %d.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Distribuição do vetor usando MPI_Scatterv
    int recvcount = local_N * sizeof(int);
    MPI_Scatterv(vector, sendcounts, displs, MPI_BYTE,
                 local_vector, recvcount, MPI_BYTE,
                 0, MPI_COMM_WORLD);

    // Liberação da memória do vetor global no rank 0
    if (rank == 0) {
        free(vector);
        free(sendcounts);
        free(displs);
    }

    // Vetor para armazenar as posições locais onde o valor foi encontrado
    long long int *local_positions = (long long int *)malloc(local_N * sizeof(long long int));
    long long int local_count = 0;

    // Deslocamento para ajustar as posições locais para posições globais
    long long int offset = 0;
    for (int i = 0; i < rank; i++) {
        offset += base_N;
        if (i < remainder) {
            offset += 1;
        }
    }

    // Busca paralela com OpenMP
    #pragma omp parallel for
    for (long long int i = 0; i < local_N; i++) {
        if (local_vector[i] == valor_buscado) {
            long long int global_pos = offset + i;
            #pragma omp critical
            {
                local_positions[local_count++] = global_pos;
            }
        }
    }

    // Preparação para enviar os resultados para o rank 0
    int *recvcounts = NULL;
    int *displs_recv = NULL;
    int local_positions_count = (int)local_count;

    // Primeiro, cada processo envia o número de posições encontradas
    int *counts = (int *)malloc(size * sizeof(int));
    MPI_Gather(&local_positions_count, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // O rank 0 prepara os arrays para receber os dados
    long long int total_positions = 0;
    if (rank == 0) {
        recvcounts = (int *)malloc(size * sizeof(int));
        displs_recv = (int *)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            recvcounts[i] = counts[i];
            displs_recv[i] = total_positions;
            total_positions += counts[i];
        }
    }

    // Coletar todas as posições encontradas no rank 0
    long long int *all_positions = NULL;
    if (rank == 0) {
        all_positions = (long long int *)malloc(total_positions * sizeof(long long int));
    }

    MPI_Gatherv(local_positions, local_positions_count, MPI_LONG_LONG_INT,
                all_positions, recvcounts, displs_recv, MPI_LONG_LONG_INT,
                0, MPI_COMM_WORLD);

    // Exibir as posições encontradas no rank 0
    if (rank == 0) {
        if (total_positions > 0) {
            printf("Valor %d encontrado nas posições:\n", valor_buscado);
            for (long long int i = 0; i < total_positions; i++) {
                printf("%lld ", all_positions[i]);
            }
            printf("\n");
        } else {
            printf("Valor %d não encontrado no vetor.\n", valor_buscado);
        }
        // Liberação da memória
        free(all_positions);
        free(recvcounts);
        free(displs_recv);
    }

    // Liberação da memória local
    free(local_vector);
    free(local_positions);
    free(counts);

    MPI_Finalize();
    return 0;
}

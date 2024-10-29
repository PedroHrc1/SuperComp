#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0                // Definindo o processo raiz
#define ARRAY_SIZE 1000000    // Tamanho total do array
#define TAG_MAX 0             // Tag para a mensagem do valor máximo

int main(int argc, char *argv[]) {
    int rank, size;
    double *array = NULL;          // Array completo (no processo raiz)
    double *sub_array = NULL;      // Sub-array para cada processo
    double max_val = 0.0;          // Valor máximo do array
    double local_max = 0.0;        // Valor máximo local
    double *normalized_array = NULL; // Array normalizado (no processo raiz)
    double *sub_normalized = NULL; // Sub-array normalizado para cada processo
    double start_time, end_time;    // Variáveis para medir o tempo de execução

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verifica se o tamanho do array é divisível pelo número de processos
    if (ARRAY_SIZE % size != 0) {
        if (rank == ROOT) {
            printf("O tamanho do array (%d) não é divisível pelo número de processos (%d).\n", ARRAY_SIZE, size);
            printf("Por favor, execute o programa com um número de processos que divida %d sem resto.\n", ARRAY_SIZE);
        }
        MPI_Finalize();
        exit(0);
    }

    int elements_per_proc = ARRAY_SIZE / size; // Número de elementos por processo

    // Alocação de memória para o sub-array
    sub_array = (double *)malloc(elements_per_proc * sizeof(double));
    if (sub_array == NULL) {
        fprintf(stderr, "Processo %d: Falha na alocação de memória para sub_array.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (rank == ROOT) {
        // Processo raiz inicializa o array com valores aleatórios
        array = (double *)malloc(ARRAY_SIZE * sizeof(double));
        if (array == NULL) {
            fprintf(stderr, "Processo %d: Falha na alocação de memória para array.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        srand(time(NULL)); // Semente para geração de números aleatórios

        // Inicializa o array com valores aleatórios entre 0 e 1000
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = ((double)rand() / RAND_MAX) * 1000.0;
        }

        // Encontra o valor máximo do array
        max_val = array[0];
        for (int i = 1; i < ARRAY_SIZE; i++) {
            if (array[i] > max_val) {
                max_val = array[i];
            }
        }

        printf("Processo %d (Raiz) encontrou o valor máximo: %.2f\n", rank, max_val);
        fflush(stdout);
    }

    // Transmite o valor máximo para todos os processos usando MPI_Bcast
    MPI_Bcast(&max_val, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    if (rank != ROOT) {
        printf("Processo %d recebeu o valor máximo: %.2f\n", rank, max_val);
        fflush(stdout);
    }

    // Distribui partes iguais do array para todos os processos usando MPI_Scatter
    MPI_Scatter(
        array,                   // Buffer de envio (no processo raiz)
        elements_per_proc,      // Número de elementos a serem enviados para cada processo
        MPI_DOUBLE,             // Tipo de dados dos elementos enviados
        sub_array,              // Buffer de recepção (em todos os processos)
        elements_per_proc,      // Número de elementos a serem recebidos
        MPI_DOUBLE,             // Tipo de dados dos elementos recebidos
        ROOT,                   // Processo raiz
        MPI_COMM_WORLD          // Comunicador
    );

    // Cada processo normaliza seu segmento do array
    sub_normalized = (double *)malloc(elements_per_proc * sizeof(double));
    if (sub_normalized == NULL) {
        fprintf(stderr, "Processo %d: Falha na alocação de memória para sub_normalized.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    start_time = MPI_Wtime(); // Inicia a medição do tempo

    for (int i = 0; i < elements_per_proc; i++) {
        sub_normalized[i] = sub_array[i] / max_val;
    }

    end_time = MPI_Wtime(); // Finaliza a medição do tempo

    printf("Processo %d: Tempo de Normalização = %.4f segundos\n", rank, end_time - start_time);
    fflush(stdout);

    // Alocação de memória para o array normalizado no processo raiz
    if (rank == ROOT) {
        normalized_array = (double *)malloc(ARRAY_SIZE * sizeof(double));
        if (normalized_array == NULL) {
            fprintf(stderr, "Processo %d: Falha na alocação de memória para normalized_array.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }

    // Coleta todos os segmentos normalizados no processo raiz usando MPI_Gather
    MPI_Gather(
        sub_normalized,          // Envio: segmento normalizado
        elements_per_proc,      // Número de elementos a serem enviados
        MPI_DOUBLE,             // Tipo de dados dos elementos enviados
        normalized_array,       // Recepção: array normalizado completo (no processo raiz)
        elements_per_proc,      // Número de elementos a serem recebidos por processo
        MPI_DOUBLE,             // Tipo de dados dos elementos recebidos
        ROOT,                   // Processo raiz
        MPI_COMM_WORLD          // Comunicador
    );

    if (rank == ROOT) {
        // Opcional: Verificar os primeiros e últimos elementos normalizados
        printf("\nProcesso %d (Raiz) recebeu o array normalizado.\n", rank);
        printf("Primeiros 10 elementos normalizados:\n");
        for (int i = 0; i < 10; i++) {
            printf("%.4f ", normalized_array[i]);
        }
        printf("\nÚltimos 10 elementos normalizados:\n");
        for (int i = ARRAY_SIZE - 10; i < ARRAY_SIZE; i++) {
            printf("%.4f ", normalized_array[i]);
        }
        printf("\n");
    }

    // Libera a memória alocada
    free(sub_array);
    free(sub_normalized);
    if (rank == ROOT) {
        free(array);
        free(normalized_array);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

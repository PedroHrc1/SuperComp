#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROOT 0                // Definindo o processo raiz
#define ARRAY_SIZE 1000000    // Tamanho total do array
#define TAG_DATA 0            // Tag para a mensagem de dados

int main(int argc, char *argv[]) {
    int rank, size;
    double *array = NULL;            // Array completo (no processo raiz)
    double *sub_array = NULL;        // Sub-array para cada processo
    double *local_means = NULL;      // Médias locais (no processo raiz)
    double *local_vars = NULL;       // Variâncias locais (no processo raiz)
    double local_sum = 0.0;          // Soma local dos elementos
    double local_sum_sq = 0.0;       // Soma local dos quadrados dos elementos
    double local_mean = 0.0;         // Média local
    double local_var = 0.0;          // Variância local
    double global_mean = 0.0;        // Média global
    double global_var = 0.0;         // Variância global
    double global_std = 0.0;         // Desvio padrão global

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

        
        // Alocação de memória para armazenar as médias e variâncias locais no processo raiz
        local_means = (double *)malloc(size * sizeof(double));
        if (local_means == NULL) {
            fprintf(stderr, "Processo %d: Falha na alocação de memória para local_means.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        local_vars = (double *)malloc(size * sizeof(double));
        if (local_vars == NULL) {
            fprintf(stderr, "Processo %d: Falha na alocação de memória para local_vars.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
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

    // Cada processo calcula a soma e a soma dos quadrados dos elementos recebidos
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += sub_array[i];
        local_sum_sq += sub_array[i] * sub_array[i];
    }

    // Calcula a média local
    local_mean = local_sum / elements_per_proc;

    // Calcula a variância local
    local_var = (local_sum_sq / elements_per_proc) - (local_mean * local_mean);


    // Coleta todas as médias locais no processo raiz usando MPI_Gather
    MPI_Gather(
        &local_mean,             // Envio: endereço da média local
        1,                       // Número de elementos a serem enviados
        MPI_DOUBLE,              // Tipo de dados
        local_means,             // Recepção: array para armazenar as médias locais (no processo raiz)
        1,                       // Número de elementos a serem recebidos por processo
        MPI_DOUBLE,              // Tipo de dados
        ROOT,                    // Processo raiz
        MPI_COMM_WORLD           // Comunicador
    );

    // Coleta todas as variâncias locais no processo raiz usando MPI_Gather
    MPI_Gather(
        &local_var,              // Envio: endereço da variância local
        1,                       // Número de elementos a serem enviados
        MPI_DOUBLE,              // Tipo de dados
        local_vars,              // Recepção: array para armazenar as variâncias locais (no processo raiz)
        1,                       // Número de elementos a serem recebidos por processo
        MPI_DOUBLE,              // Tipo de dados
        ROOT,                    // Processo raiz
        MPI_COMM_WORLD           // Comunicador
    );

    if (rank == ROOT) {
        // Calcula a média global a partir das médias locais
        double sum_means = 0.0;
        for (int i = 0; i < size; i++) {
            sum_means += local_means[i];
        }
        global_mean = sum_means / size;

        double sum_vars = 0.0;
        for (int i = 0; i < size; i++) {
            double mean_diff = local_means[i] - global_mean;
            sum_vars += (local_vars[i] + mean_diff * mean_diff);
        }
        global_var = sum_vars / size;

        // Calcula o desvio padrão global
        global_std = sqrt(global_var);

        // Imprime os resultados
        printf("\nMédias Locais Coletadas:\n");
        for (int i = 0; i < size; i++) {
            printf("Processo %d: %.2f\n", i, local_means[i]);
        }

        printf("\nVariâncias Locais Coletadas:\n");
        for (int i = 0; i < size; i++) {
            printf("Processo %d: %.2f\n", i, local_vars[i]);
        }

        printf("\nMédia Global: %.2f\n", global_mean);
        printf("Variância Global: %.2f\n", global_var);
        printf("Desvio Padrão Global: %.2f\n", global_std);
    }

    // Libera a memória alocada
    free(sub_array);
    if (rank == ROOT) {
        free(array);
        free(local_means);
        free(local_vars);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

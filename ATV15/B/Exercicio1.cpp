#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0        // Definindo o processo raiz
#define ARRAY_SIZE 100 // Tamanho total do array

int main(int argc, char *argv[]) {
    int rank, size;
    int *array = NULL;          // Array completo (no processo raiz)
    int *sub_array = NULL;      // Sub-array para cada processo
    double local_sum = 0.0;     // Soma local dos elementos
    double local_avg = 0.0;     // Média local
    double *local_avgs = NULL;  // Array para armazenar médias locais (no processo raiz)
    double global_avg = 0.0;    // Média global

    MPI_Init(&argc, &argv);                     // Inicializa o ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size);       // Obtém o número total de processos

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
    sub_array = (int *)malloc(elements_per_proc * sizeof(int));
    if (sub_array == NULL) {
        fprintf(stderr, "Processo %d: Falha na alocação de memória para sub_array.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (rank == ROOT) {
        // Processo raiz inicializa o array com valores aleatórios
        array = (int *)malloc(ARRAY_SIZE * sizeof(int));
        if (array == NULL) {
            fprintf(stderr, "Processo %d: Falha na alocação de memória para array.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        srand(time(NULL)); // Semente para geração de números aleatórios

        printf("Processo %d inicializou o array:\n", rank);
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 100; // Valores aleatórios entre 0 e 99
            printf("%d ", array[i]);
            if ((i + 1) % 10 == 0)
                printf("\n");
        }
        printf("\n");

        // Alocação de memória para armazenar as médias locais no processo raiz
        local_avgs = (double *)malloc(size * sizeof(double));
        if (local_avgs == NULL) {
            fprintf(stderr, "Processo %d: Falha na alocação de memória para local_avgs.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }

    // Distribui partes iguais do array para todos os processos
    MPI_Scatter(
        array,                   // Buffer de envio (no processo raiz)
        elements_per_proc,      // Número de elementos a serem enviados para cada processo
        MPI_INT,                 // Tipo de dados dos elementos enviados
        sub_array,               // Buffer de recepção (em todos os processos)
        elements_per_proc,      // Número de elementos a serem recebidos
        MPI_INT,                 // Tipo de dados dos elementos recebidos
        ROOT,                    // Processo raiz
        MPI_COMM_WORLD           // Comunicador
    );

    // Cada processo calcula a soma dos elementos recebidos
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += sub_array[i];
    }

    // Calcula a média local
    local_avg = local_sum / elements_per_proc;

    printf("Processo %d: Soma local = %.2f, Média local = %.2f\n", rank, local_sum, local_avg);

    // Coleta todas as médias locais no processo raiz
    MPI_Gather(
        &local_avg,             // Envio: endereço da média local
        1,                      // Número de elementos a serem enviados
        MPI_DOUBLE,             // Tipo de dados
        local_avgs,             // Recepção: array para armazenar as médias locais (no processo raiz)
        1,                      // Número de elementos a serem recebidos por processo
        MPI_DOUBLE,             // Tipo de dados
        ROOT,                   // Processo raiz
        MPI_COMM_WORLD          // Comunicador
    );

    if (rank == ROOT) {
        // Calcula a média global a partir das médias locais
        double sum_of_avgs = 0.0;
        printf("\nMédias locais coletadas:\n");
        for (int i = 0; i < size; i++) {
            printf("Processo %d: %.2f\n", i, local_avgs[i]);
            sum_of_avgs += local_avgs[i];
        }

        global_avg = sum_of_avgs / size;
        printf("\nMédia global de todos os processos: %.2f\n", global_avg);

        // Libera a memória alocada no processo raiz
        free(array);
        free(local_avgs);
    }

    // Libera a memória alocada para o sub-array
    free(sub_array);

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}

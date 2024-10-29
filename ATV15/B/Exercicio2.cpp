
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0            // Definindo o processo raiz
#define CONFIG_TAG 100    // Tag para a mensagem de configuração

int main(int argc, char *argv[]) {
    int rank, size;
    int num_iterations = 0;      // Número de iterações para o cálculo
    double local_result = 0.0;   // Resultado local do cálculo
    double start_time, end_time;  // Variáveis para medir o tempo de execução

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == ROOT) {
        // Processo raiz define a configuração
        num_iterations = 100000000; // Por exemplo, 100 milhões de iterações

        printf("Processo %d (Raiz) definiu o número de iterações: %d\n", rank, num_iterations);
        fflush(stdout);
    }

    // Transmite a configuração para todos os processos usando MPI_Bcast
    MPI_Bcast(&num_iterations, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    // Cada processo executa o cálculo com o número especificado de iterações
    // Neste exemplo, vamos calcular a soma dos números de 1 até num_iterations
    start_time = MPI_Wtime(); // Inicia a medição do tempo

    long long sum = 0;
    for (int i = 1; i <= num_iterations; i++) {
        sum += i;
    }
    local_result = (double)sum / num_iterations; // Calcula a média local

    end_time = MPI_Wtime(); // Finaliza a medição do tempo

    // Cada processo imprime seus resultados locais
    printf("Processo %d: Soma = %lld, Média Local = %.2f, Tempo de Execução = %.4f segundos\n",
           rank, sum, local_result, end_time - start_time);
    fflush(stdout);

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

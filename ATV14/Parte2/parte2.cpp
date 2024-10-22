#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h> // Inclusão da biblioteca OpenMP

int main() {
    const int N = 100000;
    int pontosDentroCirculo = 0;

    // Inicia a contagem do tempo de execução
    double inicio = omp_get_wtime();

    // Paralelização com OpenMP
    #pragma omp parallel
    {
        // Obter o ID da thread
        int thread_id = omp_get_thread_num();

        // Inicializar uma semente para o gerador de números aleatórios por thread
        unsigned int seed = static_cast<unsigned int>(time(0)) + thread_id;

        // Variável local para contagem de pontos dentro do círculo
        int pontosDentroCirculoLocal = 0;

        // Loop for paralelo com redução
        #pragma omp for reduction(+:pontosDentroCirculo)
        for (int i = 0; i < N; ++i) {
            // Geração de números aleatórios entre -1 e 1
            double x = static_cast<double>(rand_r(&seed)) / RAND_MAX * 2.0 - 1.0;
            double y = static_cast<double>(rand_r(&seed)) / RAND_MAX * 2.0 - 1.0;

            // Cálculo da distância ao quadrado até a origem
            double distanciaQuadrada = x * x + y * y;

            // Verificação se o ponto está dentro do círculo
            if (distanciaQuadrada <= 1.0) {
                ++pontosDentroCirculo;
            }
        }
    }

    // Estimativa do valor de pi
    double piEstimado = 4.0 * pontosDentroCirculo / N;

    // Cálculo do tempo de execução
    double fim = omp_get_wtime();
    double tempoExecucao = fim - inicio;

    // Exibição dos resultados
    std::cout << "Valor estimado de pi: " << piEstimado << std::endl;
    std::cout << "Tempo de execucao: " << tempoExecucao << " segundos" << std::endl;

    return 0;
}

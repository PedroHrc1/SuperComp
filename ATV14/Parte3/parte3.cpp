#include <iostream>
#include <random>   // Biblioteca para geração de números aleatórios modernos
#include <ctime>
#include <omp.h>

int main() {
    const int N = 100000;
    int pontosDentroCirculo = 0;

    // Início do tempo de execução
    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        // Obter o ID da thread
        int thread_id = omp_get_thread_num();

        // Criar um gerador de números aleatórios único para cada thread
        std::mt19937 gerador(static_cast<unsigned int>(std::time(0)) + thread_id);

        // Distribuição uniforme no intervalo [-1.0, 1.0]
        std::uniform_real_distribution<double> distribuicao(-1.0, 1.0);

        // Variável local para contagem de pontos dentro do círculo
        int pontosDentroCirculoLocal = 0;

        // Loop for paralelo com redução
        #pragma omp for reduction(+:pontosDentroCirculo)
        for (int i = 0; i < N; ++i) {
            // Geração de números aleatórios
            double x = distribuicao(gerador);
            double y = distribuicao(gerador);

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
    std::cout << "Tempo de execução: " << tempoExecucao << " segundos" << std::endl;

    return 0;
}

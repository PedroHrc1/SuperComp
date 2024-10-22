#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

int main() {
    const int N = 100000;
    int pontosDentroCirculo = 0;

    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo atual
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Inicia a contagem do tempo de execução
    clock_t inicio = std::clock();

    for (int i = 0; i < N; ++i) {
        // Gera números aleatórios entre -1 e 1 para x e y
        double x = static_cast<double>(std::rand()) / RAND_MAX * 2.0 - 1.0;
        double y = static_cast<double>(std::rand()) / RAND_MAX * 2.0 - 1.0;

        // Calcula a distância ao quadrado até a origem
        double distanciaQuadrada = x * x + y * y;

        // Verifica se o ponto está dentro do círculo de raio 1
        if (distanciaQuadrada <= 1.0) {
            ++pontosDentroCirculo;
        }
    }

    // Estima o valor de pi
    double piEstimado = 4.0 * pontosDentroCirculo / N;

    // Calcula o tempo de execução
    clock_t fim = std::clock();
    double tempoExecucao = static_cast<double>(fim - inicio) / CLOCKS_PER_SEC;

    // Exibe os resultados
    std::cout << "Valor estimado de pi: " << piEstimado << std::endl;
    std::cout << "Tempo de execucao: " << tempoExecucao << " segundos" << std::endl;

    return 0;
}

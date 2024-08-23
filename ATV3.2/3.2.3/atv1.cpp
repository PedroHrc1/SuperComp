#include <iostream>
#include <vector>
#include <chrono>     // Para medir o tempo de execução

// Tempo 0.0073448 segundos

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N, onde N é 1000.

    // Utilização de std::vector para criar uma matriz N x N.
    std::vector<std::vector<int>> matriz(N, std::vector<int>(N));

    // Preenchimento da matriz e cálculo da soma
    int soma = 0;
    auto inicio = std::chrono::high_resolution_clock::now();  // Inicia a contagem do tempo

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  // Preenche a matriz com valores (exemplo: soma dos índices)
            soma += matriz[i][j];
        }
    }

    auto fim = std::chrono::high_resolution_clock::now();  // Finaliza a contagem do tempo
    std::chrono::duration<double> duracao = fim - inicio;
    std::cout << "Soma (std::vector): " << soma << std::endl;
    std::cout << "Tempo de execução (std::vector): " << duracao.count() << " segundos" << std::endl;

    return 0;  // Retorna 0, indicando que o programa terminou com sucesso.
}

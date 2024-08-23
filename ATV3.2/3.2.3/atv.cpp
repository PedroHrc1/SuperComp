#include <iostream>
#include <chrono>     // Para medir o tempo de execução

// 0.0033099 segundos

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N, onde N é 1000.

    
    int** matriz = new int*[N];
    for (int i = 0; i < N; ++i) {
        matriz[i] = new int[N];
    }

    
    int soma = 0;
    auto inicio = std::chrono::high_resolution_clock::now();  

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  
            soma += matriz[i][j];
        }
    }

    auto fim = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> duracao = fim - inicio;
    std::cout << "Soma (alocação dinâmica manual): " << soma << std::endl;
    std::cout << "Tempo de execução (alocação dinâmica manual): " << duracao.count() << " segundos" << std::endl;

    
    for (int i = 0; i < N; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;

    return 0;  
}

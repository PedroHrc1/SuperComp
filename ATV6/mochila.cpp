#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

// Função recursiva para encontrar o valor máximo possível na mochila
int knapsackRecursive(int W, const vector<int>& weights, const vector<int>& values, int n) {
    // Caso base: sem itens ou capacidade da mochila esgotada
    if (n == 0 || W == 0)
        return 0;

    // Se o peso do item atual é maior do que a capacidade restante
    if (weights[n-1] > W)
        return knapsackRecursive(W, weights, values, n-1);

    // Retorna o valor máximo entre duas opções:
    // 1. Não incluir o item atual na mochila
    // 2. Incluir o item atual na mochila
    else
        return max(
            knapsackRecursive(W, weights, values, n-1),
            values[n-1] + knapsackRecursive(W - weights[n-1], weights, values, n-1)
        );
}

int main() {
    // Nome do arquivo de entrada
    string filename = "entrada1.txt";
    
    // Abre o arquivo de entrada
    ifstream infile(filename);

    // Verifica se o arquivo foi aberto com sucesso
    if (!infile.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return 1;
    }

    int N, W;
    infile >> N >> W;

    vector<int> weights(N);
    vector<int> values(N);

    // Leitura dos pesos e valores do arquivo
    for (int i = 0; i < N; i++) {
        infile >> weights[i] >> values[i];
    }

    // Fecha o arquivo após a leitura
    infile.close();

    // Inicia a contagem do tempo
    clock_t start = clock();

    // Chama a função recursiva
    int maxValue = knapsackRecursive(W, weights, values, N);

    // Termina a contagem do tempo
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    // Exibe o resultado e o tempo de execução
    cout << "Valor máximo na mochila: " << maxValue << endl;
    cout << "Tempo de execução: " << elapsed << " segundos" << endl;

    return 0;
}

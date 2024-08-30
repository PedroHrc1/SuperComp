#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// Função recursiva para encontrar o valor máximo possível na mochila
int knapsackRecursive(int W, const vector<int>& weights, const vector<int>& values, int n, int& pesoOcupado) {
    // Caso base: sem itens ou capacidade da mochila esgotada
    if (n == 0 || W == 0)
        return 0;

    // Se o peso do item atual é maior do que a capacidade restante
    if (weights[n-1] > W)
        return knapsackRecursive(W, weights, values, n-1, pesoOcupado);

    // Opções:
    // 1. Não incluir o item atual na mochila
    int pesoSemIncluir = pesoOcupado;
    int valorSemIncluir = knapsackRecursive(W, weights, values, n-1, pesoSemIncluir);

    // 2. Incluir o item atual na mochila
    int pesoComIncluir = pesoOcupado + weights[n-1];
    int valorComIncluir = values[n-1] + knapsackRecursive(W - weights[n-1], weights, values, n-1, pesoComIncluir);

    // Escolha a opção que maximiza o valor
    if (valorComIncluir > valorSemIncluir) {
        pesoOcupado = pesoComIncluir;
        return valorComIncluir;
    } else {
        pesoOcupado = pesoSemIncluir;
        return valorSemIncluir;
    }
}

int main() {
    // Nome do arquivo de entrada
    string filename = "entrada4.txt";
    
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

    // Variável para armazenar o peso ocupado
    int pesoOcupado = 0;

    // Chama a função recursiva
    int maxValue = knapsackRecursive(W, weights, values, N, pesoOcupado);

    // Exibe o resultado
    cout << "Peso ocupado: " << pesoOcupado << " Kg, Valor alcançado: " << maxValue << " dinheiros" << endl;

    return 0;
}

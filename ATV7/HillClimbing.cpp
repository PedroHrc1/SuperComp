#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

struct Item {
    int peso;
    int valor;
};

// Gera um vizinho modificando um bit
vector<bool> gerarVizinho(const vector<bool>& solucao) {
    vector<bool> vizinho = solucao;
    int pos = rand() % solucao.size(); // Escolhe uma posição aleatória para trocar o bit
    vizinho[pos] = !vizinho[pos];
    return vizinho;
}

// Função de Hill Climbing para o problema da mochila
int hillClimbingKnapsack(int W, const vector<Item>& itens) {
    // Gera uma solução inicial aleatória (string binária)
    vector<bool> solucao(itens.size(), false);
    int pesoOcupado = 0, valorTotal = 0;

    // Iniciar com uma solução válida (vazia)
    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        for (int i = 0; i < 10; i++) { // Gerar 10 vizinhos
            vector<bool> vizinho = gerarVizinho(solucao);

            // Calcula o peso e o valor do vizinho
            int pesoVizinho = 0, valorVizinho = 0;
            for (size_t j = 0; j < vizinho.size(); j++) {
                if (vizinho[j]) {
                    pesoVizinho += itens[j].peso;
                    valorVizinho += itens[j].valor;
                }
            }

            // Se o vizinho for melhor, adote-o como nova solução
            if (pesoVizinho <= W && valorVizinho > valorTotal) {
                solucao = vizinho;
                pesoOcupado = pesoVizinho;
                valorTotal = valorVizinho;
                melhorou = true;
            }
        }
    }

    return valorTotal;
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

    vector<Item> itens(N);

    // Leitura dos pesos e valores do arquivo
    for (int i = 0; i < N; i++) {
        infile >> itens[i].peso >> itens[i].valor;
    }

    // Fecha o arquivo após a leitura
    infile.close();

    // Chama a função de Hill Climbing
    int valorTotal = hillClimbingKnapsack(W, itens);

    // Exibe o resultado
    cout << "Valor alcançado com Hill Climbing: " << valorTotal << " dinheiros" << endl;

    return 0;
}

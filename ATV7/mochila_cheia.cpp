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

// Função para Mochila Cheia
int mochilaCheia(int W, vector<Item>& itens) {
    // Passo 1: Gera uma solução aleatória
    random_shuffle(itens.begin(), itens.end());

    int pesoOcupado = 0;
    int valorTotal = 0;

    // Passo 2: Percorra todos os itens
    for (const auto& item : itens) {
        if (pesoOcupado + item.peso <= W) {
            pesoOcupado += item.peso;
            valorTotal += item.valor;
        }
    }

    // Retorna o valor total alcançado
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

    // Chama a função de heurística
    int valorTotal = mochilaCheia(W, itens);

    // Exibe o resultado
    cout << "Valor alcançado com Mochila Cheia: " << valorTotal << " dinheiros" << endl;

    return 0;
}

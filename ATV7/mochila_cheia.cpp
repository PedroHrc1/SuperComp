#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <chrono>

using namespace std;

struct Item {
    int peso;
    int valor;
};

// Função para Mochila Cheia
pair<int, int> mochilaCheia(int W, vector<Item>& itens) {
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

    // Retorna o valor total e o peso ocupado
    return {valorTotal, pesoOcupado};
}

// Função para processar um arquivo de entrada
void processaEntrada(const string& filename) {
    // Abre o arquivo de entrada
    ifstream infile(filename);

    // Verifica se o arquivo foi aberto com sucesso
    if (!infile.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return;
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

    // Medir o tempo de execução
    auto inicio = chrono::high_resolution_clock::now();
    pair<int, int> resultado = mochilaCheia(W, itens); // valorTotal, pesoOcupado
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> duracao = fim - inicio;

    // Exibe o resultado
    cout << "Valor alcançado com Mochila Cheia para " << filename << ": " << resultado.first << " dinheiros" << endl;
    cout << "Peso ocupado: " << resultado.second << " Kg" << endl;
    cout << "Tempo de execução: " << duracao.count() << " segundos" << endl;
}

int main() {
    // Lista de nomes dos arquivos de entrada
    vector<string> filenames = {"../entrada1.txt", "../entrada2.txt", "../entrada3.txt"};

    // Processa cada arquivo de entrada e exibe o resultado
    for (const auto& filename : filenames) {
        processaEntrada(filename);
    }

    return 0;
}

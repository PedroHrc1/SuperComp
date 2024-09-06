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

// Função para verificar se a mochila pode ser melhorada substituindo um item
pair<int, int> substituicaoObjeto(int W, vector<Item>& itens) {
    // Passo 1: Mochila cheia
    random_shuffle(itens.begin(), itens.end());

    int pesoOcupado = 0;
    int valorTotal = 0;
    vector<bool> incluído(itens.size(), false); // Para marcar os itens incluídos

    // Coloca os itens inicialmente
    for (size_t i = 0; i < itens.size(); i++) {
        if (pesoOcupado + itens[i].peso <= W) {
            pesoOcupado += itens[i].peso;
            valorTotal += itens[i].valor;
            incluído[i] = true;
        }
    }

    // Passo 2: Verificar trocas
    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        for (size_t i = 0; i < itens.size(); i++) {
            if (!incluído[i]) { // Se o item não está na mochila
                for (size_t j = 0; j < itens.size(); j++) {
                    if (incluído[j] && itens[i].peso <= itens[j].peso && itens[i].valor > itens[j].valor) {
                        // Tenta substituir o item j pelo item i
                        pesoOcupado = pesoOcupado - itens[j].peso + itens[i].peso;
                        valorTotal = valorTotal - itens[j].valor + itens[i].valor;
                        incluído[i] = true;
                        incluído[j] = false;
                        melhorou = true;
                        break;
                    }
                }
            }
        }
    }

    return {valorTotal, pesoOcupado};
}

// Função para processar cada arquivo de entrada
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
    pair<int, int> resultado = substituicaoObjeto(W, itens); // valorTotal, pesoOcupado
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> duracao = fim - inicio;

    // Exibe o resultado
    cout << "Valor alcançado com Substituição de Objeto para " << filename << ": " << resultado.first << " dinheiros" << endl;
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

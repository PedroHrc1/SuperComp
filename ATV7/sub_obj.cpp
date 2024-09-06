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

// Função para verificar se a mochila pode ser melhorada substituindo um item
int substituicaoObjeto(int W, vector<Item>& itens) {
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

    // Chama a função de substituição de objeto
    int valorTotal = substituicaoObjeto(W, itens);

    // Exibe o resultado
    cout << "Valor alcançado com Substituição de Objeto: " << valorTotal << " dinheiros" << endl;

    return 0;
}
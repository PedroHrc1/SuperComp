#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

// Estrutura para armazenar peso e valor de um item
struct Item {
    int peso;
    int valor;
};

// Função para ler os dados dos arquivos de entrada
void lerEntrada(const std::string& nomeArquivo, int& capacidade, std::vector<Item>& itens) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        exit(1);
    }
    int n;
    arquivo >> n >> capacidade;
    itens.resize(n);
    for (int i = 0; i < n; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
    arquivo.close();
}

// Função da Mochila Cheia (preenche a mochila até onde for possível)
int mochilaCheia(int capacidade, std::vector<Item>& itens, int& pesoOcupado, std::vector<bool>& mochila) {
    int valorTotal = 0;
    pesoOcupado = 0;

    for (size_t i = 0; i < itens.size(); ++i) {
        if (pesoOcupado + itens[i].peso <= capacidade) {
            mochila[i] = true;  // Adiciona o item à mochila
            pesoOcupado += itens[i].peso;
            valorTotal += itens[i].valor;
        }
    }

    return valorTotal;
}

// Função para calcular o valor e o peso da mochila com base em uma solução binária
int calculaValorMochila(const std::vector<Item>& itens, const std::vector<bool>& mochila, int capacidade, int& pesoOcupado) {
    int valorTotal = 0;
    pesoOcupado = 0;

    for (size_t i = 0; i < itens.size(); ++i) {
        if (mochila[i]) {
            pesoOcupado += itens[i].peso;
            if (pesoOcupado > capacidade) {
                return 0;  // Se ultrapassar a capacidade, retorna 0
            }
            valorTotal += itens[i].valor;
        }
    }

    return valorTotal;
}

// Função de Hill Climbing para a mochila
int hillClimbingMochila(int capacidade, std::vector<Item>& itens, int& pesoOcupado, std::vector<bool>& melhorMochila) {
    // Gera uma solução inicial aleatória
    std::vector<bool> mochila(itens.size(), false);
    int valorAtual = mochilaCheia(capacidade, itens, pesoOcupado, mochila);
    
    melhorMochila = mochila;
    int melhorValor = valorAtual;

    bool melhorou = true;

    while (melhorou) {
        melhorou = false;
        for (size_t i = 0; i < itens.size(); ++i) {
            // Gera um vizinho invertendo o bit i
            mochila[i] = !mochila[i];
            int pesoOcupadoVizinho;
            int valorVizinho = calculaValorMochila(itens, mochila, capacidade, pesoOcupadoVizinho);

            // Se o vizinho for melhor, atualiza a melhor solução
            if (valorVizinho > melhorValor && pesoOcupadoVizinho <= capacidade) {
                melhorValor = valorVizinho;
                melhorMochila = mochila;
                pesoOcupado = pesoOcupadoVizinho;
                melhorou = true;
            } else {
                // Reverte a alteração se o vizinho não for melhor
                mochila[i] = !mochila[i];
            }
        }
    }

    return melhorValor;
}

int main() {
    std::srand(std::time(0));  // Inicializa o gerador de números aleatórios

    // Vetor com os nomes dos arquivos de entrada
    std::vector<std::string> arquivosEntrada = {"Entrada_1.txt", "Entrada_2.txt", "Entrada_3.txt", "Entrada_4.txt"};

    for (const auto& nomeArquivo : arquivosEntrada) {
        int capacidade;
        std::vector<Item> itens;
        lerEntrada(nomeArquivo, capacidade, itens);

        // Vetor para armazenar a melhor solução
        std::vector<bool> melhorMochila(itens.size(), false);
        int pesoOcupado = 0;

        // Mede o tempo de execução do Hill Climbing
        auto inicio = std::chrono::high_resolution_clock::now();
        int melhorValor = hillClimbingMochila(capacidade, itens, pesoOcupado, melhorMochila);
        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao = fim - inicio;

        // Saída dos resultados
        std::cout << "Arquivo: " << nomeArquivo << std::endl;
        std::cout << "Peso ocupado: " << pesoOcupado << " Kg" << std::endl;
        std::cout << "Valor máximo na mochila: " << melhorValor << " dinheiros" << std::endl;
        std::cout << "Tempo de execução: " << duracao.count() << " segundos" << std::endl;
        std::cout << "---------------------------" << std::endl;
    }

    return 0;
}
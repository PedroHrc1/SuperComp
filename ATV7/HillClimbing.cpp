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

// Função para gerar vizinhos trocando dois itens simultaneamente
std::vector<int> gerarVizinho(const std::vector<int>& solucao, int tam) {
    std::vector<int> novoVizinho = solucao;
    int pos1 = rand() % tam;
    int pos2 = rand() % tam;

    // Invertemos os valores nas duas posições escolhidas
    novoVizinho[pos1] = !novoVizinho[pos1];
    if (pos1 != pos2) {
        novoVizinho[pos2] = !novoVizinho[pos2];
    }

    return novoVizinho;
}

// Função para calcular o valor e o peso da mochila com base em uma solução
int calculaValorMochila(const std::vector<Item>& itens, const std::vector<int>& mochila, int capacidade, int& pesoOcupado) {
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

// Função de Hill Climbing modificada com exploração de dois bits
int hillClimbingDoisBits(int capacidade, std::vector<Item>& itens, int& pesoOcupado, std::vector<int>& melhorMochila, int maxIter = 1000) {
    std::vector<int> mochila(itens.size(), 0);
    int melhorValor = 0;

    // Gera uma solução inicial aleatória
    for (size_t i = 0; i < mochila.size(); ++i) {
        mochila[i] = rand() % 2;
    }

    melhorMochila = mochila;
    melhorValor = calculaValorMochila(itens, mochila, capacidade, pesoOcupado);

    bool melhorou = true;
    int iter = 0;

    // Realiza o Hill Climbing
    while (melhorou && iter < maxIter) {
        melhorou = false;
        ++iter;

        // Gera um vizinho trocando dois bits aleatórios
        std::vector<int> vizinho = gerarVizinho(melhorMochila, itens.size());
        int pesoOcupadoVizinho;
        int valorVizinho = calculaValorMochila(itens, vizinho, capacidade, pesoOcupadoVizinho);

        // Se o vizinho for melhor, atualiza a melhor solução
        if (valorVizinho > melhorValor && pesoOcupadoVizinho <= capacidade) {
            melhorValor = valorVizinho;
            melhorMochila = vizinho;
            pesoOcupado = pesoOcupadoVizinho;
            melhorou = true;
        }
    }

    return melhorValor;
}

int main() {
    std::srand(std::time(0));  // Inicializa o gerador de números aleatórios

    // Vetor com os nomes dos arquivos de entrada
    std::vector<std::string> arquivosEntrada = {"../entrada1.txt", "../entrada2.txt", "../entrada3.txt"};

    for (const auto& nomeArquivo : arquivosEntrada) {
        int capacidade;
        std::vector<Item> itens;
        lerEntrada(nomeArquivo, capacidade, itens);

        // Vetor para armazenar a melhor solução
        std::vector<int> melhorMochila(itens.size(), 0);
        int pesoOcupado = 0;

        // Mede o tempo de execução do Hill Climbing modificado
        auto inicio = std::chrono::high_resolution_clock::now();
        int melhorValor = hillClimbingDoisBits(capacidade, itens, pesoOcupado, melhorMochila);
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

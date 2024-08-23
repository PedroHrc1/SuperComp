#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.

void lerArquivo(const std::string& nomeArquivo, std::vector<int>& numeros) {
    std::ifstream arquivo(nomeArquivo);
    int numero;
    while (arquivo >> numero) {
        numeros.push_back(numero);
    }
    arquivo.close();
}


void dobrarValoresReferencia(const std::vector<int>& numerosOriginais, std::vector<int>& numerosDobrados) {
    numerosDobrados.clear();  // Limpa o vetor de destino
    for (int numero : numerosOriginais) {
        numerosDobrados.push_back(numero * 2);
    }
}


void dobrarValoresPonteiro(const std::vector<int>* numerosOriginais, std::vector<int>* numerosDobrados) {
    numerosDobrados->clear();  // Limpa o vetor de destino
    for (int numero : *numerosOriginais) {
        numerosDobrados->push_back(numero * 2);
    }
}


void escreverArquivo(const std::string& nomeArquivo, const std::vector<int>& numeros) {
    std::ofstream arquivo(nomeArquivo);
    for (int numero : numeros) {
        arquivo << numero << std::endl;
    }
    arquivo.close();
}

int main() {
    std::vector<int> numeros, numerosDobrados;

    // Chama a função para ler os números do arquivo "entrada.txt" e armazena-os no vetor 'numeros'.
    lerArquivo("entrada.txt", numeros);

    // Chama a função que dobra os valores do vetor 'numeros' usando passagem por referência e armazena o resultado no vetor 'numerosDobrados'.
    dobrarValoresReferencia(numeros, numerosDobrados);

    // Chama a função que escreve os números do vetor 'numerosDobrados' no arquivo "saida_referencia.txt".
    escreverArquivo("saida_referencia.txt", numerosDobrados);

    // Chama a função que dobra os valores do vetor 'numeros' usando passagem por ponteiro e armazena o resultado no vetor 'numerosDobrados'.
    dobrarValoresPonteiro(&numeros, &numerosDobrados);

    // Chama a função que escreve os números do vetor 'numerosDobrados' no arquivo "saida_ponteiro.txt".
    escreverArquivo("saida_ponteiro.txt", numerosDobrados);

    return 0;
}

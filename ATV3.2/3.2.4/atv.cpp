#include <iostream>
#include <vector>

class Vector {
public:
    
    Vector(int tamanho) : tam(0), capacidade(tamanho) {
        dados = new int[capacidade];  
    }

    
    ~Vector() {
        delete[] dados;
    }

    
    void inicializa(int valor) {
        for (int i = 0; i < capacidade; ++i) {
            dados[i] = valor;
        }
        tam = capacidade;  
    }

   
    int get(int index) const {
        if (index >= 0 && index < tam) {
            return dados[index];
        } else {
            std::cerr << "Erro: Índice fora dos limites" << std::endl;
            return -1;  
        }
    }

    
    void set(int index, int valor) {
        if (index >= 0 && index < tam) {
            dados[index] = valor;
        } else {
            std::cerr << "Erro: Índice fora dos limites" << std::endl;
        }
    }

    
    void inserir(int index, int valor) {
        if (tam >= capacidade) {
            redimensiona(capacidade * 2);  
        }
        if (index >= 0 && index <= tam) {
            for (int i = tam; i > index; --i) {
                dados[i] = dados[i - 1];
            }
            dados[index] = valor;
            ++tam;
        } else {
            std::cerr << "Erro: Índice fora dos limites" << std::endl;
        }
    }

    
    void remover(int index) {
        if (index >= 0 && index < tam) {
            for (int i = index; i < tam - 1; ++i) {
                dados[i] = dados[i + 1];
            }
            --tam;
        } else {
            std::cerr << "Erro: Índice fora dos limites" << std::endl;
        }
    }

    
    void imprime() const {
        for (int i = 0; i < tam; ++i) {
            std::cout << dados[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    int* dados;       
    int tam;          
    int capacidade;   

    
    void redimensiona(int novaCapacidade) {
        int* novoDados = new int[novaCapacidade];
        for (int i = 0; i < tam; ++i) {
            novoDados[i] = dados[i];
        }
        delete[] dados;
        dados = novoDados;
        capacidade = novaCapacidade;
    }
};

int main() {
    Vector vec(5);
    vec.inicializa(0);   // Inicializa todos os elementos com 0
    vec.imprime();       // Imprime o vetor

    vec.set(2, 10);      // Modifica o valor no índice 2
    vec.imprime();       // Imprime o vetor

    vec.inserir(1, 20);  // Insere o valor 20 no índice 1
    vec.imprime();       // Imprime o vetor

    vec.remover(3);      // Remove o elemento no índice 3
    vec.imprime();       // Imprime o vetor

    return 0;
}

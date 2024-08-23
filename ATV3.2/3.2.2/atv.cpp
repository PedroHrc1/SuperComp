#include <iostream>
#include <vector>


class Matriz {
public:
    
    Matriz(int N) {
        data.resize(N, std::vector<int>(N, 0));  
    }

    
    int get(int i, int j) const {
        if (i >= 0 && i < tamanho() && j >= 0 && j < tamanho()) {
            return data[i][j];
        } else {
            std::cerr << "Erro: Índice fora dos limites ao acessar get(" << i << ", " << j << ")\n";
            return -1;  // Valor de erro
        }
    }

    
    void set(int i, int j, int valor) {
        if (i >= 0 && i < tamanho() && j >= 0 && j < tamanho()) {
            data[i][j] = valor;
        } else {
            std::cerr << "Erro: Índice fora dos limites ao acessar set(" << i << ", " << j << ")\n";
        }
    }

    
    void imprime() const {
        for (const auto& linha : data) {
            for (int valor : linha) {
                std::cout << valor << " ";
            }
            std::cout << std::endl;
        }
    }

    
    int tamanho() const {
        return data.size();
    }

private:
    std::vector<std::vector<int>> data;  
};


void multiplicaMatriz(const Matriz& A, const Matriz& B, Matriz& C) {
    int N = A.tamanho();  

    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int soma = 0;
            for (int k = 0; k < N; k++) {
                
                soma += A.get(i, k) * B.get(k, j);
            }
            
            C.set(i, j, soma);
        }
    }
}

int main() {
    int N = 3;  // Define o tamanho da matriz (N x N)

    // Cria as matrizes: A, B, e C, todas de dimensão N x N
    Matriz A(N), B(N), C(N);

    // Exemplo de preenchimento das matrizes A e B
    // Sinta-se à vontade para modificar os valores
    A.set(0, 0, 1); A.set(0, 1, 2); A.set(0, 2, 3);
    A.set(1, 0, 4); A.set(1, 1, 5); A.set(1, 2, 6);
    A.set(2, 0, 7); A.set(2, 1, 8); A.set(2, 2, 9);

    B.set(0, 0, 9); B.set(0, 1, 8); B.set(0, 2, 7);
    B.set(1, 0, 6); B.set(1, 1, 5); B.set(1, 2, 4);
    B.set(2, 0, 3); B.set(2, 1, 2); B.set(2, 2, 1);

    // Multiplica as matrizes A e B, armazenando o resultado em C
    multiplicaMatriz(A, B, C);

    // Imprime a matriz resultante C
    C.imprime();

    return 0;  // Retorna 0, indicando que o programa terminou com sucesso
}

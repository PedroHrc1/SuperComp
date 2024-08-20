#include <iostream>
#include <vector>


int main(){

    std::vector<std::vector<int>> matriz;

    for (int i = 0; i < 3; i++)
    {
        std::vector<int> linha;
        for (int j = 0; j < 3; j++)
        {
            int n;
            std::cout << "Digite um número: ";
            std::cin >> n;
            linha.push_back(n);
        }
        matriz.push_back(linha);
    }

    int soma = 0;

    for (int i = 0; i < 3; i++)
    {
        soma += matriz[i][i];
    }

    std::cout << "A soma dos elementos da diagonal principal é: " << soma << std::endl;

}
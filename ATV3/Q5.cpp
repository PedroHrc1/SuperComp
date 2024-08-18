#include <iostream>
#include <vector>


int main(){

    std::vector<int> numeros;

    int n;

    for (int i = 0; i < 5; i++)
    {
        std::cout << "Digite um número: ";
        std::cin >> n;

        numeros.push_back(n);
    }

    std::cout << "O seu vetor ficou assim:" << numeros[0] << ", " << numeros[1] << ", " << numeros[2] << ", " << numeros[3] << ", " << numeros[4] << std::endl;
    std::cout << "A soma dos números é: " << numeros[0] + numeros[1] + numeros[2] + numeros[3] + numeros[4] << std::endl;

}
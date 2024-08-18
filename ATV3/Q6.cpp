#include <iostream>
#include <vector>

int main(){

    std::vector<int> numeros;

    int n;
    int maior;

    for (int i = 0; i < 10; i++)
    {
        std::cout << "Digite um número: ";
        std::cin >> n;

        numeros.push_back(n);
    }

    for (int y = 1; y < numeros.size(); y++)
    {
        if(numeros[y] > numeros[y-1]){
            maior = numeros[y];
        }
    }
    

    std::cout << "O maior número é: " << maior << std::endl;

}
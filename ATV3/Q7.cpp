#include <iostream>
#include <vector>

int main(){

    std::vector<int> numeros;

    int n;

    for (int i = 0; i <= 20; i++)
    {
        std::cout << "Digite um número: ";
        std::cin >> n;

        numeros.push_back(n);
    }

    //Bobble Sort
    for (int i = 0; i < numeros.size(); i++)
    {
        for (int j = 0; j < numeros.size(); j++)
        {
            if (numeros[i] < numeros[j])
            {
                int temp = numeros[i];
                numeros[i] = numeros[j];
                numeros[j] = temp;
            }
        }
    }

    std::cout << "O seu vetor ficou assim:"; 

   //Codigo para printar o vetor usando um for

    for (int i = 0; i < numeros.size(); i++)
    {
        std::cout << numeros[i] << ", ";
    }

    


}


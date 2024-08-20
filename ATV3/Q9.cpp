#include <iostream>

//Escreva um programa que verifique se uma string inserida pelo usuário é um palíndromo (lê-se da mesma forma de trás para frente).

int main(){

    std::string palavra;
    std::cout << "Digite uma palavra: ";
    std::cin >> palavra;

    bool palindromo = true;

    for (int i = 0; i < palavra.size(); i++)
    {
        if (palavra[i] != palavra[palavra.size() - 1 - i])
        {
            palindromo = false;
            break;
        }
    }

    if (palindromo)
    {
        std::cout << "A palavra é um palíndromo" << std::endl;
    }
    else
    {
        std::cout << "A palavra não é um palíndromo" << std::endl;
    }
    
}
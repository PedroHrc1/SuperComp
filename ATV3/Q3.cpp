#include <iostream>

int main(){

    int n;

    std::cout << "Digite um número: ";
    std::cin >> n;

    if (n % 2 == 0)
    {
        std::cout << "O número " << n << " é par." << std::endl;
    }
    else
    {
        std::cout << "O número " << n << " é ímpar." << std::endl;
    }

}
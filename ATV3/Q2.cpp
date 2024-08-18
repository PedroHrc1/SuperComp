#include <iostream>

int main() {
    
    int n;
    int n1;
    
    std::string operador;

    bool i = true;

    while (i)
    {
        std::cout << "Digite um número: ";
        std::cin >> n;

        std::cout << "Digite um operador(+,-,*,/): ";
        std::cin >> operador;

        std::cout << "Digite outro número: ";
        std::cin >> n1;

        if (operador == "+")
        {
            std::cout << n << " + " << n1 << " = " << n + n1 << std::endl;
            i = false;
        }
        else if (operador == "-")
        {
            std::cout << n << " - " << n1 << " = " << n - n1 << std::endl;
            i = false;
        }
        else if (operador == "*")
        {
            std::cout << n << " * " << n1 << " = " << n * n1 << std::endl;
            i = false;
        }
        else if (operador == "/")
        {
            std::cout << n << " / " << n1 << " = " << n / n1 << std::endl;
            i = false;
        }
        else
        {
            std::cout << "Operador inválido!" << std::endl;
        }


        
    }
    
    
}
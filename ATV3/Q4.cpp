#include <iostream>

int main(){

    std::string palavra;

    std::cout << "Digite uma palavra: ";
    std::cin >> palavra;

    std::cout << "A palavra " << palavra << " tem " << palavra.size() << " letras." << std::endl;


}
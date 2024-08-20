#include <iostream>
#include <vector>

//Crie um programa que simule um sistema de controle de estoque para uma loja. O programa deve permitir ao
// usuário adicionar itens, remover itens, e visualizar o estoque atual. Os dados devem serarmazenados em vetores.

int main(){

    std::vector<std::string> produtos;
    std::vector<int> quantidades;

    while (true)
    {
        std::cout << "1 - Adicionar item" << std::endl;
        std::cout << "2 - Remover item" << std::endl;
        std::cout << "3 - Visualizar estoque" << std::endl;
        std::cout << "4 - Sair" << std::endl;

        int opcao;
        std::cin >> opcao;

        if (opcao == 1)
        {
            std::string produto;
            int quantidade;

            std::cout << "Digite o nome do produto: ";
            std::cin >> produto;

            std::cout << "Digite a quantidade: ";
            std::cin >> quantidade;

            produtos.push_back(produto);
            quantidades.push_back(quantidade);
        }
        else if (opcao == 2)
        {
            std::string produto;
            std::cout << "Digite o nome do produto que deseja remover: ";
            std::cin >> produto;

            for (int i = 0; i < produtos.size(); i++)
            {
                if (produtos[i] == produto)
                {
                    produtos.erase(produtos.begin() + i);
                    quantidades.erase(quantidades.begin() + i);
                    break;
                }
            }
        }
        else if (opcao == 3)
        {
            for (int i = 0; i < produtos.size(); i++)
            {
                std::cout << produtos[i] << " - " << quantidades[i] << std::endl;
            }
        }
        else if (opcao == 4)
        {
            break;
        }
    }
    
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

/*
Estatísticas de Texto: Escreva um programa que leia um arquivo de texto e calcule as seguintes
estatísticas: número total de palavras, número total de linhas, número médio de palavras por linha e a palavra mais frequente.
Em seguida, grave essas estatísticas em um novo arquivo de texto.
*/

int main() {
    // Open the input file
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cout << "Failed to open input file." << std::endl;
        return 1;
    }

    // Open the output file
    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cout << "Failed to open output file." << std::endl;
        return 1;
    }

    // Variables to store statistics
    int totalWords = 0;
    int totalLines = 0;
    double averageWordsPerLine = 0.0;
    std::string mostFrequentWord;
    int maxWordFrequency = 0;
    std::unordered_map<std::string, int> wordFrequency;

    // Read the input file line by line
    std::string line;
    while (std::getline(inputFile, line)) {
        totalLines++;

        // Count the number of words in the line
        std::istringstream iss(line);
        std::string word;
        int wordCount = 0;
        while (iss >> word) {
            totalWords++;
            wordCount++;

            // Update the frequency of the current word
            wordFrequency[word]++;
            if (wordFrequency[word] > maxWordFrequency) {
                maxWordFrequency = wordFrequency[word];
                mostFrequentWord = word;
            }
        }
    }

    // Calculate average words per line
    if (totalLines > 0) {
        averageWordsPerLine = static_cast<double>(totalWords) / totalLines;
    }

    // Write statistics to the output file
    outputFile << "Total Words: " << totalWords << std::endl;
    outputFile << "Total Lines: " << totalLines << std::endl;
    outputFile << "Average Words Per Line: " << averageWordsPerLine << std::endl;
    outputFile << "Most Frequent Word: " << mostFrequentWord << " (" << maxWordFrequency << " times)" << std::endl;

    return 0;
}
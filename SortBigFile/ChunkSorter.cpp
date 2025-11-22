#include "ChunkSorter.h"
#include <fstream>
#include <iostream>
#include <algorithm>

std::vector<std::string> ChunkSorter::splitAndSort(
    const std::string& inputFile,
    size_t chunkSize)
{
    std::ifstream in(inputFile);
    if (!in) {
        std::cout << "Ошибка: невозможно открыть файл " << inputFile << "\n";
        return {};
    }

    std::vector<std::string> chunkFiles;
    std::vector<int> buffer;
    buffer.reserve(chunkSize);

    int number;
    size_t chunkIndex = 0;

    // Требование задания: использовать while (!fs.eof())
    while (!in.eof()) {
        buffer.clear();

        // Считываем chunkSize чисел
        for (size_t i = 0; i < chunkSize && (in >> number); ++i) {
            buffer.push_back(number);
        }

        if (buffer.empty())
            break;

        // Сортируем кусок в памяти
        std::sort(buffer.begin(), buffer.end());

        // Записываем результат в новый файл
        std::string chunkName = "chunk_" + std::to_string(chunkIndex++) + ".txt";
        std::ofstream out(chunkName, std::ios::trunc); // trunc по заданию

        for (int value : buffer) {
            out << value << "\n";
        }

        out.close();
        chunkFiles.push_back(chunkName);

        std::cout << "Создан отсортированный кусок: " << chunkName << "\n";
    }

    return chunkFiles;
}
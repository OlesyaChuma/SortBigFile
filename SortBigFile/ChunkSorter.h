#pragma once

#include <string>
#include <vector>

class ChunkSorter {
public:
    // Разбивает исходный файл на маленькие отсортированные части
    static std::vector<std::string> splitAndSort(
        const std::string& inputFile,
        size_t chunkSize
    );
};

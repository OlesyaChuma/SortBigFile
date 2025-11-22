#pragma once

#include <string>
#include <vector>

class FileMerger {
public:
    // Слияние двух отсортированных файлов в один
    static std::string mergeTwo(
        const std::string& f1,
        const std::string& f2,
        const std::string& out
    );

    // Попарное слияние всех файлов, пока не останется один
    static std::string mergeAll(const std::vector<std::string>& files);
};


#include "FileMerger.h"
#include "Utils.h"

#include <fstream>
#include <iostream>

std::string FileMerger::mergeTwo(
    const std::string& f1,
    const std::string& f2,
    const std::string& outFile)
{
    std::ifstream A(f1);
    std::ifstream B(f2);

    std::ofstream out(outFile, std::ios::trunc);

    if (!A || !B || !out) {
        std::cout << "Ошибка открытия файлов при слиянии!\n";
        return "";
    }

    int x, y;

    // Читаем первые значения
    bool aOK = static_cast<bool>(A >> x);
    bool bOK = static_cast<bool>(B >> y);

    // Основное слияние
    while (aOK && bOK) {
        if (x < y) {
            out << x << "\n";
            aOK = static_cast<bool>(A >> x);
        }
        else {
            out << y << "\n";
            bOK = static_cast<bool>(B >> y);
        }
    }

    // Записываем остатки
    while (aOK) {
        out << x << "\n";
        aOK = static_cast<bool>(A >> x);
    }

    while (bOK) {
        out << y << "\n";
        bOK = static_cast<bool>(B >> y);
    }

    A.close();
    B.close();
    out.close();

    deleteFile(f1);
    deleteFile(f2);

    return outFile;
}
std::string FileMerger::mergeAll(const std::vector<std::string>& files)
{
    if (files.empty())
        return "";

    // Текущий набор файлов
    std::vector<std::string> current = files;

    // Пока не останется только один файл
    while (current.size() > 1) {
        std::vector<std::string> next;

        for (size_t i = 0; i < current.size(); i += 2) {
            // Нечётный последний файл просто переносим дальше
            if (i + 1 >= current.size()) {
                next.push_back(current[i]);
                break;
            }

            // Имя нового файла после слияния
            std::string output =
                "merged_" + std::to_string(i / 2) + ".txt";

            // Слияние пары файлов
            std::string merged = mergeTwo(current[i], current[i + 1], output);
            next.push_back(merged);

            std::cout << "Файлы " << current[i] << " и " << current[i + 1]
                << " → " << output << "\n";
        }

        current = next;
    }

    return current[0]; // итоговый файл
}
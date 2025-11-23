#include "FileMerger.h"
#include "Utils.h"

#include <fstream>
#include <iostream>

std::string FileMerger::mergeTwo(
    const std::string& f1,
    const std::string& f2,
    const std::string& outFile)
{
    std::cout << "Слияние файлов: " << f1 << " + " << f2 << std::endl;

    std::ifstream A(f1);
    std::ifstream B(f2);
    std::ofstream out(outFile, std::ios::trunc);

    // Проверка ошибок открытия
    if (!A) {
        std::cout << "[ОШИБКА] Не удалось открыть файл A: " << f1 << std::endl;
        return "";
    }
    if (!B) {
        std::cout << "[ОШИБКА] Не удалось открыть файл B: " << f2 << std::endl;
        return "";
    }
    if (!out) {
        std::cout << "[ОШИБКА] Не удалось создать выходной файл: " << outFile << std::endl;
        return "";
    }

    int x, y;

    // Первое чтение
    bool aOK = static_cast<bool>(A >> x);
    bool bOK = static_cast<bool>(B >> y);

    // Основной цикл слияния
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

    // Дописываем остатки
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

    // ❗ВАЖНО: НЕ УДАЛЯЕМ файлы здесь!
    // Их удалит mergeAll ПОСЛЕ того, как убедится,
    // что файл больше не участвует в слиянии.

    std::cout << "Готово: " << outFile << std::endl;

    return outFile;
}

std::string FileMerger::mergeAll(const std::vector<std::string>& files)
{
    if (files.empty())
        return "";

    std::vector<std::string> current = files;

    while (current.size() > 1) {

        std::vector<std::string> next;
        next.reserve((current.size() + 1) / 2);

        for (size_t i = 0; i < current.size(); i += 2) {

            // Если файл один — переносим в следующий цикл
            if (i + 1 == current.size()) {
                next.push_back(current[i]);
                continue;
            }

            // Безопасные имена файлов
            std::string f1 = current[i];
            std::string f2 = current[i + 1];

            if (f1.empty() || f2.empty()) {
                std::cout << "[ОШИБКА] Один из путей пуст! f1='"
                    << f1 << "' f2='" << f2 << "'\n";
                continue;
            }

            // Имя для нового объединённого файла
            std::string outName = "merged_" + std::to_string(next.size()) + ".txt";

            std::cout << "Слияние файлов: " << f1 << " + " << f2 << std::endl;

            std::string merged = mergeTwo(f1, f2, outName);

            if (merged.empty()) {
                std::cout << "[ОШИБКА] Ошибка при слиянии " << f1
                    << " и " << f2 << std::endl;
                return "";
            }

            next.push_back(merged);
        }

        current = next;
    }

    return current[0];
}
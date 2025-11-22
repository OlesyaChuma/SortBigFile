// SortBigFile.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <locale>
#include <chrono>

#include "FileGenerator.h"
#include "ChunkSorter.h"
#include "FileMerger.h"

int main() {
    setlocale(LC_ALL, "");

    auto mark = [](std::string name, auto start, auto end) {
        std::cout << name << ": "
            << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
            << " сек\n";
        };

    auto t0 = std::chrono::steady_clock::now();

    const std::string INPUT = "bigfile.txt";
    const size_t NUMBERS = 500'000'000;   // ~2 ГБ для int (4 байта)
    const size_t CHUNK_SIZE = 1'000'000;  // 4 МБ — помещается в ОЗУ

    // 1. Генерация большого файла (2 ГБ)
    FileGenerator::generate(INPUT, NUMBERS);
    auto t1 = std::chrono::steady_clock::now();
    mark("Генерация файла", t0, t1);

    // 2. Разбиваем файл на отсортированные части
    auto t2 = std::chrono::steady_clock::now();
    auto chunks = ChunkSorter::splitAndSort(INPUT, CHUNK_SIZE);
    auto t3 = std::chrono::steady_clock::now();
    mark("Разбиение на куски", t2, t3);

    // 3. Попарно сливаем части в один большой отсортированный файл
    auto t4 = std::chrono::steady_clock::now();
    std::string finalFile = FileMerger::mergeAll(chunks);
    auto t5 = std::chrono::steady_clock::now();
    mark("Слияние файлов", t4, t5);

    std::cout << "\nГОТОВО! Итоговый отсортированный файл: "
        << finalFile << "\n";

    return 0;
}
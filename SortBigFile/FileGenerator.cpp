#include "FileGenerator.h"
#include <fstream>
#include <random>
#include <iostream>

void FileGenerator::generate(const std::string& filename, size_t numbersCount) {

    // trunc — подсказка №1: очистить файл, если существует
    std::ofstream out(filename, std::ios::trunc);
    if (!out) {
        std::cout << "Ошибка: невозможно создать файл " << filename << "\n";
        return;
    }

    // mt19937_64 - современный, быстрый, качественный генератор псевдослучайных чисел (PRNG)
    // Создаётся временный random_device, который генерирует одно случайное число, и оно передаётся как seed
    std::mt19937_64 gen(std::random_device{}());
    // Это класс распределения, который задаёт: диапазон случайных чисел от 0 до 1 000 000 000
    std::uniform_int_distribution<int> dist(0, 1'000'000'000);

    // Записываем numbersCount случайных чисел
    for (size_t i = 0; i < numbersCount; ++i) {
        out << dist(gen) << "\n";// dist(gen) вернёт случайное число
    }

    std::cout << "Файл " << filename << " успешно создан.\n";
}
// SortBigFile.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <locale>
#include <chrono>
#include <windows.h>

#include "Utils.h"
#include "FileGenerator.h"
#include "ChunkSorter.h"
#include "FileMerger.h"

// Удаление всех CHUNK-файлов (chunk_*.txt)
void cleanupChunkFiles(const std::vector<std::string>& files,
    const std::string& finalFile)
{
    for (const auto& f : files) {
        if (f != finalFile) {
            deleteFile(f);
        }
    }
}

// Удаление всех MERGED-файлов (merged_*.txt), кроме итогового
void cleanupMergedFiles(const std::string& finalFile)
{
    // структура, в которую Windows запишет информацию о найденном файле
    // Буква A означает ANSI-версию (FindFirstFileA)
    WIN32_FIND_DATAA data;

    // Эта функция ищет первый файл в папке, который подходит под маску
    // и заполняет структуру data информацией о первом найденном файле
    HANDLE h = FindFirstFileA("merged_*.txt", &data);

    // Если найден хотя бы один файл — возвращает дескриптор поиска (HANDLE).
    // Если таких файлов нет — возвращает INVALID_HANDLE_VALUE.
    
    if (h == INVALID_HANDLE_VALUE)
        return;

    do {
        // Обрабатываем файл в data.cFileName
        // цикл перебирает все файлы, подходящие под маску
        std::string name = data.cFileName;

        if (name != finalFile) {
            deleteFile(name);
        }

    } while (FindNextFileA(h, &data));

    FindClose(h);
}

// Функция форматирования времени(HH:MM:SS)
std::string formatDuration(std::chrono::steady_clock::duration d) {
    using namespace std::chrono;

    auto secs = duration_cast<seconds>(d).count();

    int hours = secs / 3600;
    secs %= 3600;

    int minutes = secs / 60;
    secs %= 60;

    int seconds = secs;

    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return std::string(buffer);
}

int main() {
    setlocale(LC_ALL, "");

    auto mark = [](const std::string& name, auto start, auto end) {
        std::cout << name << ": "
            << formatDuration(end - start)  
            << "\n";
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

    // 4. Удаление временных файлов (все chunk_*.txt)
    cleanupChunkFiles(chunks, finalFile);

    // 5. Удаляем merged-файлы
    cleanupMergedFiles(finalFile);

    std::cout << "\nГОТОВО! Итоговый отсортированный файл: "
        << finalFile << "\n";

    return 0;
}
#pragma once

#include <string>
#include <cstdio>

// Удаление файла (подсказка №2)
inline void deleteFile(const std::string& fname) {
    std::remove(fname.c_str());
}
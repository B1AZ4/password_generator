/*
main.c - основная программа

Смоликов Роман
МК-101
*/
#include "password_generator.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif
    Config config;
    if (!parse_arguments(argc, argv, &config)) {
        return 1;
    }
    generate_passwords(&config);
    return 0;
}
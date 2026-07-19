/*
main.c - основная программа

Смоликов Роман
МК-101
*/
#include "password_generator.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    Config config;
    if (!parse_arguments(argc, argv, &config)) {
        return 1;
    }
    generate_passwords(&config);
    return 0;
}
/*
password_generator.h - заголовочный файл модуля password_generator.c

Смоликов Роман
МК-101
*/
#ifndef PASSWORDGEN_H
#define PASSWORDGEN_H
#include <stdbool.h>

#define MAX_DELIMITERS 256
#define MAX_ALPHABET 512

typedef struct {
    int minl;
    int maxl;
    int length;
    int count;
    char alphabet[MAX_ALPHABET];
    char charset_flags[16];

    bool has_minl;
    bool has_maxl;
    bool has_n;
    bool has_c;
    bool has_a;
    bool has_C;
} Config;

bool parse_arguments(int argc, char *argv[], Config* config);
void generate_passwords(const Config* config);

#endif
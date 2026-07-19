/*
 Модуль для работы с аргументами командной строки и генерацией пароля

Смоликов Роман
МК-101
*/
#include "password_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static bool is_delimiter(char c, const char* delims) {
    for (int i = 0; delims[i] != '\0'; i++) {
        if (delims[i] == c) return true;
    }
    return false;
}

static bool is_numeric(const char* str) {
    if (str == NULL || strlen(str) == 0) return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) return false;
    }
    return true;
}
bool parse_arguments(int argc, char* argv[], Config* config) {
    config->minl = -1; config->maxl = -1; config->length = -1; config->count = 1;
    config->alphabet[0] = '\0'; config->charset_flags[0] = '\0';
    config->has_minl = config->has_maxl = config->has_n = config->has_c = config->has_a = config->has_C = false;

    char delims[MAX_DELIMITERS] = "=:";

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (strncmp(arg, "-D", 2) == 0) {
            char* val = NULL;
            if (strlen(arg) > 2) {
                if (is_delimiter(arg[2], "=:")) val = &arg[3];
                else val = &arg[2];
            }
            else if (i + 1 < argc) {
                val = argv[++i];
            }
            if (val == NULL || strlen(val) != 1) {
                fprintf(stderr, "Ошибка:Некорректное значение для -D\n");
                return false;
            }
            delims[0] = val[0]; delims[1] = '\0';
        }
        else if (strncmp(arg, "-d", 2) == 0) {
            char* val = NULL;
            if (strlen(arg) > 2) {
                if (is_delimiter(arg[2], delims)) val = &arg[3];
                else val = &arg[2];
            }
            else if (i + 1 < argc) {
                val = argv[++i];
            }
            if (val == NULL || strlen(val) != 1) {
                fprintf(stderr, "Ошибка:Некорректное значение для -d\n");
                return false;
            }
            if (!is_delimiter(val[0], delims) && strlen(delims) < MAX_DELIMITERS - 1) {
                int len = strlen(delims);
                delims[len] = val[0]; delims[len + 1] = '\0';
            }
        }
    }
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (strncmp(arg, "-d", 2) == 0 || strncmp(arg, "-D", 2) == 0) {
            if (strlen(arg) == 2) i++;
            continue;
        }

        char* opt = NULL;
        char* val = NULL;
        int opt_len = 0;

        if (strncmp(arg, "-minl", 5) == 0) { opt = "-minl"; opt_len = 5; }
        else if (strncmp(arg, "-maxl", 5) == 0) { opt = "-maxl"; opt_len = 5; }
        else if (strncmp(arg, "-n", 2) == 0) { opt = "-n"; opt_len = 2; }
        else if (strncmp(arg, "-c", 2) == 0) { opt = "-c"; opt_len = 2; }
        else if (strncmp(arg, "-a", 2) == 0) { opt = "-a"; opt_len = 2; }
        else if (strncmp(arg, "-C", 2) == 0) { opt = "-C"; opt_len = 2; }
        else {
            continue;
        }
        if (strlen(arg) > opt_len) {
            if (is_delimiter(arg[opt_len], delims)) {
                val = arg + opt_len + 1; //-minl=10
            }
            else {
                val = arg + opt_len;     //-minl10
            }
        }
        else if (i + 1 < argc) {
            val = argv[++i];             //-minl 10
        }

        if (val == NULL || strlen(val) == 0) {
            fprintf(stderr, "Ошибка: Отсутствует значение для опции %s\n", opt);
            return false;
        }

        // Проверка
        if (strcmp(opt, "-minl") == 0) {
            if (config->has_minl) { fprintf(stderr, "Ошибка: Повторяющаяся опция -minl\n"); return false; }
            if (!is_numeric(val)) { fprintf(stderr, "Ошибка: Значение -minl должно быть числом\n"); return false; }
            config->minl = atoi(val); config->has_minl = true;
        }
        else if (strcmp(opt, "-maxl") == 0) {
            if (config->has_maxl) { fprintf(stderr, "Ошибка: Повторяющаяся опция -maxl\n"); return false; }
            if (!is_numeric(val)) { fprintf(stderr, "Ошибка: Значение -maxl должно быть числом\n"); return false; }
            config->maxl = atoi(val); config->has_maxl = true;
        }
        else if (strcmp(opt, "-n") == 0) {
            if (config->has_n) { fprintf(stderr, "Ошибка: Повторяющаяся опция -n\n"); return false; }
            if (!is_numeric(val)) { fprintf(stderr, "Ошибка: Значение -n должно быть числом\n"); return false; }
            config->length = atoi(val); config->has_n = true;
        }
        else if (strcmp(opt, "-c") == 0) {
            if (config->has_c) { fprintf(stderr, "Ошибка: Повторяющаяся опция -c\n"); return false; }
            if (!is_numeric(val)) { fprintf(stderr, "Ошибка: Значение -c должно быть числом\n"); return false; }
            config->count = atoi(val); config->has_c = true;
        }
        else if (strcmp(opt, "-a") == 0) {
            if (config->has_a) { fprintf(stderr, "Ошибка: Повторяющаяся опция -a\n"); return false; }
            strcpy(config->alphabet, val); config->has_a = true;
        }
        else if (strcmp(opt, "-C") == 0) {
            if (config->has_C) { fprintf(stderr, "Ошибка: Повторяющаяся опция -C\n"); return false; }
            //Флаги {a, A, D, S}
            for (int k = 0; val[k] != '\0'; k++) {
                if (val[k] != 'a' && val[k] != 'A' && val[k] != 'D' && val[k] != 'S') {
                    fprintf(stderr, "Ошибка: Недопустимый символ '%c' в наборе -C\n", val[k]);
                    return false;
                }
            }
            strcpy(config->charset_flags, val); config->has_C = true;
        }
    }
    if (config->has_minl != config->has_maxl) {
        fprintf(stderr, "Ошибка: Опции -minl и -maxl должны быть указаны только вместе\n");
        return false;
    }
    if (config->has_n && (config->has_minl || config->has_maxl)) {
        fprintf(stderr, "Ошибка: Опции -n и -minl/-maxl несовместимы\n");
        return false;
    }
    if (config->has_a && config->has_C) {
        fprintf(stderr, "Ошибка: Опции -a и -C несовместимы\n");
        return false;
    }
    if (config->has_minl && config->minl > config->maxl) {
        fprintf(stderr, "Ошибка: Минимальная длина (-minl) не может быть больше максимальной (-maxl)\n");
        return false;
    }
    if (!config->has_n && !config->has_minl) {
        config->length = 8;
        config->has_n = true;
    }

    return true;
}
void generate_passwords(const Config* config) {
    char symbols[MAX_ALPHABET] = "";

    if (config->has_a) {
        strcpy(symbols, config->alphabet);
    }
    else {
        if (!config->has_C) {
            strcat(symbols, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        }
        else {
            if (strchr(config->charset_flags, 'a')) strcat(symbols, "abcdefghijklmnopqrstuvwxyz");
            if (strchr(config->charset_flags, 'A')) strcat(symbols, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            if (strchr(config->charset_flags, 'D')) strcat(symbols, "0123456789");
            if (strchr(config->charset_flags, 'S')) strcat(symbols, "!@#$%^&*()-_=+[{]};:',<.>/?");
        }
    }

    int alphabet_len = strlen(symbols);
    if (alphabet_len == 0) {
        fprintf(stderr, "Ошибка: Итоговый алфавит для генерации пуст\n");
        return;
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < config->count; i++) {
        int len = config->length;

        if (config->has_minl) {
            len = config->minl + rand() % (config->maxl - config->minl + 1);
        }

        for (int j = 0; j < len; j++) {
            int random_index = rand() % alphabet_len;
            putchar(symbols[random_index]);
        }
        putchar('\n');
    }
}
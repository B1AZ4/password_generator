/*
 Модуль для работы с аргументами командной строки и генерацией пароля

Смоликов Роман
МК-101
*/
#include "pwdgen.h"
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
    return true;
}
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
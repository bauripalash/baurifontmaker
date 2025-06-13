#include "include/utils.h"
#include "include/ext/raylib.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void SeedRandom() { srand((unsigned)time(NULL)); }

bool RandomBool() { return (rand() & 1) != 0; }

bool IsValidHex(const char *str) {
    if (strcmp(str, "") == 0) {
        return false;
    }

    size_t startIndex = 0;
    size_t inputLen = (size_t)strlen(str);

    if (strlen(str) >= 2 &&
        (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))) {
        startIndex = 2;
    }

    for (size_t i = startIndex; i < inputLen; i++) {
        if (!isxdigit(str[i])) {
            return false;
        }
    }

    return true;
}

Rectangle GetCenteredRect(int width, int height) {
    return (Rectangle){.x = (GetScreenWidth() - width) * 0.5f,
                       .y = (GetScreenHeight() - height) * 0.5f,
                       .width = width,
                       .height = height};
}

bool IsLinux() {
#ifdef BFM_OS_LINUX
    return true;
#else
    return true;
#endif
}
bool IsWin() {
#ifdef BFM_OS_WIN
    return true;
#else
    return false;
#endif
}
bool IsMac() {
#ifdef BFM_OS_MAC
    return true;
#else
    return false;
#endif
}
bool IsWeb() {
#ifdef BFM_OS_WEB
    return true;
#else
    return false;
#endif
}

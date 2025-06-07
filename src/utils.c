#include "include/utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

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

    for (size_t i = 0; i < inputLen; i++) {
        if (!isxdigit(str[i])) {
            return false;
        }
    }

    return true;
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

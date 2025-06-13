#ifndef UTILS_H
#define UTILS_H
#include "ext/raylib.h"
#include <stdbool.h>

#define OS_NAME_LINUX 0
#define OS_NAME_WIN   1
#define OS_NAME_MAC   2
#define OS_NAME_WEB   3

void SeedRandom();
bool IsValidHex(const char *str);
bool RandomBool();

// Os Check
// Injected defination from cmake or zig build system
bool IsLinux();
bool IsWin();
bool IsMac();
bool IsWeb();

Rectangle GetCenteredRect(int width, int height);

#endif

#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#define OS_NAME_LINUX 0
#define OS_NAME_WIN   1
#define OS_NAME_MAC   2
#define OS_NAME_WEB   3

bool IsValidHex(const char *str);

// Os Check
// Injected defination from cmake or zig build system
bool IsLinux();
bool IsWin();
bool IsMac();
bool IsWeb();

#endif

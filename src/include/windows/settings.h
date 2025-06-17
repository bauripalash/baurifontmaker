#ifndef WIN_SETTINGS_H
#define WIN_SETTINGS_H

#include "../ext/raylib.h"
#include <stdbool.h>

#define SETTINGS_WIN_WIDTH  500
#define SETTINGS_WIN_HEIGHT 400

typedef struct {
    bool windowActive;
    Rectangle windowBounds;
    bool enableGrid;
    char configPath[512];
    int canvasRow;
    int canvasColumn;
} SettingsState;

SettingsState CreateSettingsState();
bool SettingsWindow(SettingsState *state);
void LogSettings(const SettingsState *state);

#endif

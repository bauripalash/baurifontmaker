#include "../include/windows/settings.h"
#include "../include/colors.h"
#include "../include/defaults.h"
#include "../include/ext/raygui.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WIN_MARGIN         20
#define PADDING            10
#define LABEL_W            200
#define LABEL_H            32
#define CHECKBOX_SIZE      16
#define GRID_THICKNESS_MIN 0
#define GRID_THICKNESS_MAX 50
#define SPINNER_WIDTH      100

SettingsState CreateSettingsState() {
    SettingsState state = {};
    state.windowActive = false;
    state.windowBounds =
        GetCenteredRect(SETTINGS_WIN_WIDTH, SETTINGS_WIN_HEIGHT);
    state.enableGrid = true;
    state.canvasColumn = DEF_CANVAS_COLUMN;
    state.canvasRow = DEF_CANVAS_ROW;
    strcpy(state.configPath, "");

    return state;
}

void LogSettings(const SettingsState *state) {
    printf("== Settings ==\n");
    printf("Enable Grid => %s\n", state->enableGrid ? "true" : "false");
    printf(
        "Canvas Size => Column:%d, Row:%d\n", state->canvasColumn,
        state->canvasRow
    );
    printf("Config Path => \"%s\"\n", state->configPath);
    printf("==============\n");
}

bool Checkbox(const Rectangle rect, bool *checked) {
    int iconId = ICON_CROSS;
    if (*checked) {
        iconId = ICON_OK_TICK;
    }
    int ogStyle = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, HexColorBackground);
    bool clicked = GuiButton(rect, GuiIconText(iconId, NULL));
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ogStyle);

    if (clicked) {
        *checked = !*checked;
    }

    return checked;
}

bool SettingsWindow(SettingsState *state) {
    Rectangle bound = GetCenteredRect(SETTINGS_WIN_WIDTH, SETTINGS_WIN_HEIGHT);

    int posX = bound.x;
    int posY = bound.y + 25;
    state->windowBounds.x = bound.x;
    state->windowBounds.y = bound.y;

    if (state->windowActive) {
        state->windowActive = !GuiWindowBox(state->windowBounds, "Settings");
        GuiGroupBox(
            (Rectangle){
                posX + WIN_MARGIN,
                posY + WIN_MARGIN,
                SETTINGS_WIN_WIDTH - WIN_MARGIN * 2,
                100,
            },
            "~canvas"
        );

        posY += PADDING + WIN_MARGIN;

        GuiLabel(
            (Rectangle){
                posX + WIN_MARGIN + PADDING,
                posY,
                LABEL_W,
                LABEL_H,
            },
            "Enable Grid"
        );

        Checkbox(
            (Rectangle){
                posX + SETTINGS_WIN_WIDTH - CHECKBOX_SIZE - WIN_MARGIN -
                    PADDING,
                posY,
                CHECKBOX_SIZE,
                CHECKBOX_SIZE,
            },
            &state->enableGrid
        );

        posY += WIN_MARGIN + 100 + PADDING;
        posX += WIN_MARGIN;

        bool saveClicked = GuiButton(
            (Rectangle){
                posX,
                posY,
                100,
                LABEL_H,
            },
            GuiIconText(ICON_FILE_SAVE, "Save")
        );

        posX += WIN_MARGIN + 100 + PADDING * 2;

        bool cancelClicked = GuiButton(
            (Rectangle){posX, posY, 100, LABEL_H},
            GuiIconText(ICON_EXIT, "Cancel")
        );

        if (saveClicked) {
            state->windowActive = false;
            return true;
        }

        if (cancelClicked) {
            state->windowActive = false;
            return false;
        }
    }

    return false;
}

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "../ext/raylib.h"
#include <stdbool.h>

#define TOOL_MARGIN  5
#define BUTTON_WIDTH 80

typedef struct {
    bool enable;
    Rectangle bounds;
    int toolbarHeight;
    bool newBtnClicked;
    bool openBtnClicked;
    bool saveBtnClicked;
    bool settingsBtnClicked;
    bool helpBtnClicked;
} ToolbarState;

ToolbarState CreateToolbar(int height);
void SetToolbarHeight(ToolbarState *state, int height);
void Toolbar(ToolbarState *state);

#endif

#include "include/toolbar.h"
#include "include/ext/raygui.h"
#include "include/ext/raylib.h"
#include <stdbool.h>
#include <string.h>

ToolbarState CreateToolbar(int height) {
    return (ToolbarState){
        .enable = true,
        .bounds =
            (Rectangle){
                0,
                0,
                GetScreenWidth(),
                height,
            },
        .toolbarHeight = height,
        .newBtnClicked = false,
        .openBtnClicked = false,
        .saveBtnClicked = false,
        .settingsBtnClicked = false,
        .helpBtnClicked = false,
    };
}

void SetToolbarHeight(ToolbarState *state, int height) {
    state->bounds.height = height;
}

void Toolbar(ToolbarState *state) {
    state->bounds.width = GetScreenWidth();
    GuiPanel(state->bounds, NULL);
    Rectangle btnBound = {
        TOOL_MARGIN, TOOL_MARGIN, BUTTON_WIDTH,
        state->bounds.height - (TOOL_MARGIN * 2)
    };

    int leftIncrement = btnBound.width + (TOOL_MARGIN * 2);

    state->newBtnClicked =
        GuiButton(btnBound, GuiIconText(ICON_FILE_NEW, "~new"));

    btnBound.x += leftIncrement;

    state->openBtnClicked =
        GuiButton(btnBound, GuiIconText(ICON_FILE_OPEN, "~open"));

    btnBound.x += leftIncrement;

    state->saveBtnClicked =
        GuiButton(btnBound, GuiIconText(ICON_FILE_SAVE, "~save"));

    btnBound.x = state->bounds.width - TOOL_MARGIN - btnBound.width;

    state->helpBtnClicked = GuiButton(btnBound, GuiIconText(ICON_HELP, NULL));
    btnBound.x -= TOOL_MARGIN * 2 + btnBound.width;
    state->settingsBtnClicked =
        GuiButton(btnBound, GuiIconText(ICON_GEAR, NULL));
}

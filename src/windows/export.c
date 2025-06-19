#include "../include/windows/export.h"
#include "../include/balloc.h"
#include "../include/codegen.h"
#include "../include/ext/raygui.h"
#include "../include/objects/glyph.h"
#include <stdbool.h>
#include <stdlib.h>

ExportState CreateExportState() {
    ExportState state = {0};
    state.windowActive = false;
    state.windowBounds = (Rectangle){0, 0, EXPORT_W_WIDTH, EXPORT_W_HEIGHT};
    state.buffer = NULL;
    state.codeBtnClicked = false;

    return state;
}

void ClearExportBuffer(ExportState *state) {
    if (state->buffer != NULL) {
        bfree(state->buffer);
        state->buffer = NULL;
    }
}
#define BTN_W 80
#define BTN_H 30

bool ExportWindow(ExportState *state, GlyphObj *obj) {

    int winposX = (GetScreenWidth() - EXPORT_W_WIDTH) * 0.5f;
    int winposY = (GetScreenHeight() - EXPORT_W_HEIGHT) * 0.5f;
    int winW = state->windowBounds.width;
    int winH = state->windowBounds.height;

    state->windowBounds.x = winposX;
    state->windowBounds.y = winposY;

    if (state->windowActive) {
        state->windowActive = !GuiWindowBox(
            state->windowBounds,
            TextFormat(
                "%s Export to File : %s", GuiIconText(ICON_FILE_EXPORT, NULL),
                obj->name
            )
        );

        int x = winposX + EXPORT_W_MARGIN;
        int y = winposY + EXPORT_W_MARGIN;
        state->codeBtnClicked =
            GuiButton((Rectangle){x, y, BTN_W, BTN_H}, "Code");

        x += EXPORT_W_MARGIN + BTN_W;

        bool fcode = GuiButton((Rectangle){x, y, BTN_W, BTN_H}, "Font");

        x = winposX + EXPORT_W_MARGIN;
        y += BTN_H + EXPORT_W_MARGIN;
        Rectangle codeBoxRect = (Rectangle){
            x,
            y,
            winW - EXPORT_W_MARGIN * 2,
            winH - BTN_H - EXPORT_W_MARGIN * 3,

        };

        if (state->buffer == NULL) {
            GuiDisable();
        }
        GuiTextBox(codeBoxRect, state->buffer, 1024, false);
        if (state->buffer == NULL) {
            GuiEnable();
        }
    }

    return false;
}

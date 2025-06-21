#include "../include/windows/export.h"
#include "../external/raygui.h"
#include "../include/balloc.h"
#include "../include/colors.h"
#include "../include/glyph.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

ExportState CreateExportState() {
    ExportState state = {0};
    state.windowActive = false;
    state.windowBounds = (Rectangle){0, 0, EXPORT_W_WIDTH, EXPORT_W_HEIGHT};
    state.buffer = NULL;
    state.saveBtnClicked = false;
    state.scrollOffset = (Vector2){0, 0};
    state.scrollView = (Rectangle){0};
    state.font = GuiGetFont();
    state.codeHeight = 20;
    state.langSelectActive = 0;
    state.langSelectEdit = false;
    state.typeSelectActive = false;
    state.typeSelectActive = 0;
    state.fontTypeActive = 0;
    state.fontTypeEdit = false;

    return state;
}

void ClearExportBuffer(ExportState *state) {
    if (state->buffer != NULL) {
        bfree(state->buffer);
        state->buffer = NULL;
    }
}
#define BTN_W   80
#define BTN_H   30
#define SPACING 2
#define LINE_H  20

// See
// https://github.com/raysan5/rguilayout/blob/7749831c79107d4ecc69f5389ea41937e52f62f4/src/gui_window_codegen.h#L205
void ScrollTextWidget(ExportState *state, Rectangle bounds, const char *text) {
    Rectangle contentRect = {
        bounds.x, bounds.y, bounds.width * 2, state->codeHeight
    };
    Rectangle view = (Rectangle){0};

    GuiScrollPanel(bounds, NULL, contentRect, &state->scrollOffset, &view);

    BeginScissorMode(view.x, view.y, view.width, view.height);
    size_t lc = 0;
    char *curline = state->buffer;

    while (curline) {
        char *nextline = strchr(curline, '\n');
        if (nextline) {
            *nextline = '\0';
        }

        if (((state->scrollOffset.y + LINE_H * lc) >= -40) &&
            ((state->scrollOffset.y + LINE_H * lc) < (bounds.height - 2))) {
            DrawTextEx(
                state->font, curline,
                (Vector2){
                    bounds.x + state->scrollOffset.x + 10, // what is 10?
                    bounds.y + state->scrollOffset.y + LINE_H * lc + 8,

                },
                state->font.baseSize, SPACING, ColorPrimary
            );
        }

        if (nextline) {
            *nextline = '\n';
        }

        curline = nextline ? (nextline + 1) : NULL;
        lc++;
    }

    state->codeHeight = LINE_H * lc;

    EndScissorMode();
}

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
        int y = winposY + EXPORT_W_MARGIN + 10;
        Rectangle typeSelectRect = {
            x,
            y,
            200,
            32,
        };
        // state->codeBtnClicked =
        //     GuiButton((Rectangle){x, y, BTN_W, BTN_H}, "Code");

        x += EXPORT_W_MARGIN + 200;

        Rectangle dropDownRect = {x, y, 200, BTN_H};

        x += EXPORT_W_MARGIN + 200;

        Rectangle saveBtnRect = {x, y, BTN_W, BTN_H};
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

        ScrollTextWidget(state, codeBoxRect, state->buffer);
        if (state->buffer == NULL) {
            GuiEnable();
        }

        if (state->typeSelectActive == TYPE_SEL_CODE) {
            if (GuiDropdownBox(
                    dropDownRect, LANG_SEL_STR, &state->langSelectActive,
                    state->langSelectEdit
                )) {
                state->langSelectEdit = !state->langSelectEdit;
            }
        } else if (state->typeSelectActive == TYPE_SEL_FONT) {
            if (GuiDropdownBox(
                    dropDownRect, FONT_SEL_STR, &state->fontTypeActive,
                    state->fontTypeEdit
                )) {
                state->fontTypeEdit = !state->fontTypeEdit;
            }
        }

        if (GuiDropdownBox(
                typeSelectRect, TYPE_SEL_STR, &state->typeSelectActive,
                state->typeSelectEdit
            )) {
            state->typeSelectEdit = !state->typeSelectEdit;
        }

        state->saveBtnClicked =
            GuiButton(saveBtnRect, GuiIconText(ICON_FILE_SAVE, "Save File"));

        if (state->saveBtnClicked) {
            // DO STUFF HERE
            state->saveBtnClicked = false; // to remove?
            state->windowActive = false;
        }
    }

    return false;
}

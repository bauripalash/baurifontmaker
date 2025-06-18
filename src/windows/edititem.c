#include "../include/windows/edititem.h"
#include "../include/ext/raygui.h"
#include "../include/utils.h"

#include <stdbool.h>
#include <string.h>

#define INP_H     32
#define LBL_WIDTH 200

EditItemState CreateEditItemState() {
    EditItemState state = {};
    state.windowActive = false;
    state.windowBounds = (Rectangle){
        0,
        0,
        EDIT_W_WIDTH,
        EDIT_W_HEIGHT,
    };

    state.errWinX = 0;
    state.errWinY = 0;

    state.invalidHexError = false;
    state.invalidIndexError = false;

    strcpy(state.hexStr, "");
    strcpy(state.nameStr, "");

    state.indexValue = 0;

    return state;
}

void SetStateEditItem(EditItemState *state, GlyphItem *target, int itemLen) {
    strcpy(state->hexStr, TextFormat("0x%x", target->value));
    strcpy(state->nameStr, target->name);
    state->indexValue = target->listIndex;
}

int EditItemWindow(EditItemState *state, GlyphItem *target, int itemLen) {
    state->windowBounds.x = (GetScreenWidth() - EDIT_W_WIDTH) * 0.5f;
    state->windowBounds.y = (GetScreenHeight() - EDIT_W_HEIGHT) * 0.5f;
    int winPosX = state->windowBounds.x;
    int winPosY = state->windowBounds.y;

    if (state->windowActive) {
        if (state->invalidHexError || state->invalidIndexError) {
            GuiLock();
        }
        state->windowActive = !GuiWindowBox(
            state->windowBounds,
            TextFormat(
                "%s Edit Font Item : %s", GuiIconText(ICON_PENCIL, NULL),
                target->name
            )
        );

        int y = winPosY + EDIT_W_MARGIN;
        GuiLabel(
            (Rectangle){winPosX + EDIT_W_MARGIN, y, LBL_WIDTH, INP_H},
            "Edit Hex : "
        );

        if (GuiTextBox(
                (Rectangle){
                    winPosX + EDIT_W_MARGIN + LBL_WIDTH,
                    y,
                    LBL_WIDTH,
                    INP_H,
                },
                state->hexStr, 17, state->hexEditMode
            )) {
            state->hexEditMode = !state->hexEditMode;
        }

        y += INP_H + 5;

        GuiLabel(
            (Rectangle){winPosX + EDIT_W_MARGIN, y, 200, 32}, "Edit Name :"
        );

        if (GuiTextBox(
                (Rectangle){
                    winPosX + EDIT_W_MARGIN + 200,
                    y,
                    200,
                    32,
                },
                state->nameStr, 128, state->nameEditMode
            )) {
            state->nameEditMode = !state->nameEditMode;
        }

        y += INP_H + 5;

        GuiLabel(
            (Rectangle){winPosX + EDIT_W_MARGIN, y, 200, 32}, "Edit Index :"
        );

        if (GuiSpinner(
                (Rectangle){winPosX + EDIT_W_MARGIN + LBL_WIDTH, y, LBL_WIDTH,
                            INP_H},
                NULL, &state->indexValue, 0, itemLen - 1, state->indexEditMode
            )) {
            state->indexEditMode = !state->indexEditMode;
        }
        y += INP_H * 2;
        int x = winPosX + EDIT_W_MARGIN;
        bool saveClicked = GuiButton(
            (Rectangle){
                x,
                y,
                80,
                INP_H,

            },
            GuiIconText(ICON_FILE_SAVE, "Save")
        );

        x += 80 + EDIT_W_MARGIN;

        bool removeClicked = GuiButton(
            (Rectangle){x, y, 90, INP_H}, GuiIconText(ICON_BIN, "Remove")
        );

        x += 90 + EDIT_W_MARGIN;

        bool cancelClicked = GuiButton(
            (Rectangle){x, y, 90, INP_H}, GuiIconText(ICON_EXIT, "Cancel")
        );

        if (cancelClicked) {
            state->windowActive = false;
            return EDIT_CANCEL_CLICK;
        }

        if (saveClicked) {
            if (!IsValidHex(state->hexStr)) {
                state->invalidHexError = true;
            } else {
                state->windowActive = false;
            }
            return EDIT_SAVE_CLICK;
        }

        if (removeClicked) {
            state->windowActive = false;
            return EDIT_REMOVE_CLICK;
        }

        if (state->invalidHexError) {
            GuiUnlock();
            const char *title =
                GuiIconText(ICON_WARNING, "Error Creating Font Item");
            state->errWinX = (GetScreenWidth() - EDIT_ERR_WIDTH) * 0.5f;
            state->errWinY = state->windowBounds.y +
                             20; //(GetScreenHeight() - NW_ERR_HEIGHT) * 0.5f;
            int val = GuiMessageBox(
                (Rectangle){
                    state->errWinX,
                    state->errWinY,
                    EDIT_ERR_WIDTH,
                    EDIT_ERR_HEIGHT,
                },
                title, "Invalid Hex code", "Close"
            );
            if (val != -1) {
                state->invalidHexError = false;
            }
        }
    }

    return -1;
}

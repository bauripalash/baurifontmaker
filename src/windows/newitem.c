#include "../include/windows/newitem.h"
#include <stdbool.h>
#include <string.h>

NewItemWindowState CreateNewItemWindow() {
    NewItemWindowState state = {};
    state.windowActive = false;
    state.windowBounds = (Rectangle){
        0,
        0,
        NW_WIDTH,
        NW_HEIGHT,
    };

    strcpy(state.hexCode, "");
    strcpy(state.itemName, "");
    state.hexEditMode = false;
    state.nameEditMode = false;
    state.invalidHexError = false;

    state.errWinX = 0;
    state.errWinY = 0;

    return state;
}

void CleanNewItemState(NewItemWindowState *state) {
    strcpy(state->hexCode, "");
    strcpy(state->itemName, "");
}

bool NewItemWindow(NewItemWindowState *state) {
    state->windowBounds.x = (GetScreenWidth() - NW_WIDTH) * 0.5f;
    state->windowBounds.y = (GetScreenHeight() - NW_HEIGHT) * 0.5f;
    int errWinX = (GetScreenWidth() - NW_ERR_WIDTH) * 0.5f;
    int errWinY = (GetScreenHeight() - NW_ERR_HEIGHT) * 0.5f;
    int winPosX = state->windowBounds.x;
    int winPosY = state->windowBounds.y;
    if (state->windowActive) {
        if (state->invalidHexError) {
            GuiLock();
        }
        state->windowActive =
            !GuiWindowBox(state->windowBounds, "Create New Font Item");

        int y = winPosY + NW_MARGIN;
        GuiLabel(
            (Rectangle){winPosX + NW_MARGIN, y, 200, 32}, "Font Item Hex : "
        );

        if (GuiTextBox(
                (Rectangle){winPosX + NW_MARGIN + 200, y, 200, 32},
                state->hexCode, MAX_HEXCODE, state->hexEditMode
            )) {
            state->hexEditMode = !state->hexEditMode;
        }

        y += 32 + 5;
        GuiLabel(
            (Rectangle){winPosX + NW_MARGIN, y, 200, 32}, "Font Item Name :"
        );

        if (GuiTextBox(
                (Rectangle){
                    winPosX + NW_MARGIN + 200,
                    y,
                    200,
                    32,
                },
                state->itemName, MAX_NAME, state->nameEditMode
            )) {
            state->nameEditMode = !state->nameEditMode;
        }

        bool createClicked = GuiButton(
            (Rectangle){winPosX + NW_MARGIN, winPosY + NW_MARGIN + 100, 80, 32},
            "Create"
        );

        bool cancelClicked = GuiButton(
            (Rectangle){winPosX + NW_MARGIN + 80 + NW_MARGIN,
                        winPosY + NW_MARGIN + 100, 80, 32},
            "Cancel"
        );

        if (createClicked) {
            if (!TextIsEqual(state->hexCode, "0x01")) {
                state->invalidHexError = true;
            } else {
                state->windowActive = false;
                return true;
            }
        }
        if (cancelClicked) {
            state->windowActive = false;
        }

        if (state->invalidHexError) {
            GuiUnlock();
            const char *title =
                GuiIconText(ICON_WARNING, "Error Creating Font Item");
            state->errWinX = (GetScreenWidth() - NW_ERR_WIDTH) * 0.5f;
            state->errWinY = state->windowBounds.y +
                             20; //(GetScreenHeight() - NW_ERR_HEIGHT) * 0.5f;
            int val = GuiMessageBox(
                (Rectangle){
                    state->errWinX,
                    state->errWinY,
                    NW_ERR_WIDTH,
                    NW_ERR_HEIGHT,
                },
                title, "Invalid Hex code", "Close"
            );
            if (val != -1) {
                state->invalidHexError = false;
            }
        }
    }

    return false;
}

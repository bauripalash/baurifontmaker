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

    return state;
}

void cleanData(NewItemWindowState *state) {
    memset(state->hexCode, '\0', sizeof(state->hexCode));
}

bool NewItemWindow(NewItemWindowState *state) {
    state->windowBounds.x = (GetScreenWidth() - NW_WIDTH) * 0.5f;
    state->windowBounds.y = (GetScreenHeight() - NW_HEIGHT) * 0.5f;
    int winPosX = state->windowBounds.x;
    int winPosY = state->windowBounds.y;
    if (state->windowActive) {
        // cleanData(state);
        state->windowActive =
            !GuiWindowBox(state->windowBounds, "Create New Font Item");

        int y = winPosY + NW_MARGIN;
        GuiLabel(
            (Rectangle){winPosX + NW_MARGIN, y, 200, 32}, "Font Item Hex : "
        );

        int val = GuiTextBox(
            (Rectangle){winPosX + NW_MARGIN + 200, y, 200, 32}, state->hexCode,
            120, true
        );

        y += 32 + 5;
        GuiLabel(
            (Rectangle){winPosX + NW_MARGIN, y, 200, 32}, "Font Item Name :"
        );

        val = GuiTextBox(
            (Rectangle){
                winPosX + NW_MARGIN + 200,
                y,
                200,
                32,
            },
            state->itemName, 120, true
        );

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
            // cleanData(state);
            state->windowActive = false;

            return true;
        }
        if (cancelClicked) {
            state->windowActive = false;
        }
    }

    return false;
}

#include "../include/windows/newitem.h"

NewItemWindowState CreateNewItemWindow() {
    NewItemWindowState state = {};
    state.windowActive = false;
    state.windowBounds = (Rectangle){
        (GetScreenWidth() - NW_WIDTH) * 0.5f,
        (GetScreenHeight() - NW_HEIGHT) * 0.5f,
        NW_WIDTH,
        NW_HEIGHT,
    };

    return state;
}

bool NewItemWindow(NewItemWindowState *state) {
    int winPosX = state->windowBounds.x;
    int winPosY = state->windowBounds.y;
    if (state->windowActive) {
        state->windowActive =
            !GuiWindowBox(state->windowBounds, "Create New Font Item");

        GuiLabel((Rectangle){winPosX + NW_MARGIN, winPosY + NW_MARGIN, 200, 32},
                 "Font Item Hex : ");

        int val = GuiTextBox((Rectangle){winPosX + NW_MARGIN + 200,
                                         winPosY + NW_MARGIN, 200, 32},
                             state->hexCode, 120, true);

        bool createClicked = GuiButton(
            (Rectangle){winPosX + NW_MARGIN, winPosY + NW_MARGIN + 100, 80, 32},
            "Create");

        bool cancelClicked =
            GuiButton((Rectangle){winPosX + NW_MARGIN + 80 + NW_MARGIN,
                                  winPosY + NW_MARGIN + 100, 80, 32},
                      "Cancel");

        if (createClicked) {
            state->windowActive = false;
            return true;
        }
        if (cancelClicked) {
            state->windowActive = false;
        }
    }

    return false;
}

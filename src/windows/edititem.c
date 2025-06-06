#include "../include/windows/edititem.h"
#include <stdbool.h>

EditItemWindowState CreateEditItemWindow() {
    EditItemWindowState state = {};
    state.windowActive = false;
    state.windowBounds = (Rectangle){
        (GetScreenWidth() - EDIT_W_WIDTH) * 0.5f,
        (GetScreenHeight() - EDIT_W_HEIGHT) * 0.5f,
        EDIT_W_WIDTH,
        EDIT_W_HEIGHT,
    };

    return state;
}

bool EditItemWindow(EditItemWindowState *state, FontItem *target) {
    int winPosX = state->windowBounds.x;
    int winPosY = state->windowBounds.y;

    if (state->windowActive) {
        state->windowActive = !GuiWindowBox(
            state->windowBounds, TextFormat("Edit Font Item : %s", target->name)
        );
    }

    return false;
}

#include "../include/windows/infoedit.h"
#include "../include/ext/raygui.h"
#include "../include/objects/glyph.h"
#include <stdbool.h>
#include <string.h>

#define LABEL_WIDTH  200
#define INPUT_WIDTH  300
#define LABEL_HEIGHT 32
#define PADDING      10

InfoEditState CreateInfoEditState() {
    InfoEditState state = {};
    state.windowActive = false;
    state.windowBounds = (Rectangle){
        0,
        0,
        INFO_W_WIDTH,
        INFO_W_HEIGHT,
    };
    state.nameEditMode = false;
    state.authorEditMode = false;
    state.licenseEditMode = false;
    state.descEditMode = false;

    return state;
}

void UpdateInfo(InfoEditState *state, const GlyphObj *obj) {
    if (!TextIsEqual(state->name, obj->name)) {
        TextCopy(state->name, obj->name);
    }

    if (!TextIsEqual(state->author, obj->author)) {
        TextCopy(state->author, obj->author);
    }

    if (!TextIsEqual(state->license, obj->license)) {
        TextCopy(state->license, obj->license);
    }

    if (!TextIsEqual(state->description, obj->description)) {
        TextCopy(state->description, obj->description);
    }
}

// should we let gui handle this process?
void updateGlyphInfo(InfoEditState *state, GlyphObj *obj) {
    if (!TextIsEqual(state->name, obj->name)) {
        TextCopy(obj->name, state->name);
    }

    if (!TextIsEqual(state->author, obj->author)) {
        TextCopy(obj->author, state->author);
    }

    if (!TextIsEqual(state->license, obj->license)) {
        TextCopy(obj->license, state->license);
    }

    if (!TextIsEqual(state->description, obj->description)) {
        TextCopy(obj->description, state->description);
    }
}

bool InfoEditWindow(InfoEditState *state, GlyphObj *obj) {
    int winposX = (GetScreenWidth() - INFO_W_WIDTH) * 0.5f;
    int winposY = (GetScreenHeight() - INFO_W_HEIGHT) * 0.5f;
    int winW = state->windowBounds.width;
    int winH = state->windowBounds.height;

    state->windowBounds.x = winposX;
    state->windowBounds.y = winposY;

    if (state->windowActive) {
        state->windowActive = !GuiWindowBox(
            state->windowBounds,
            TextFormat(
                "%s Edit Font Information : %s",
                GuiIconText(ICON_FILETYPE_INFO, NULL), obj->name
            )
        );

        int x = winposX + INFO_W_MARGIN;
        int y = winposY + INFO_W_MARGIN;
        int inpX = winposX + winW - INFO_W_MARGIN - INPUT_WIDTH;
        Rectangle btnRect = {x, y, LABEL_WIDTH, LABEL_HEIGHT};

        Rectangle inpRect = {inpX, y, INPUT_WIDTH, LABEL_HEIGHT};

        GuiLabel(btnRect, "Name");

        if (GuiTextBox(
                inpRect, state->name, MAX_GLYPH_NAME_LEN, state->nameEditMode
            )) {
            state->nameEditMode = !state->nameEditMode;
        }

        btnRect.y += PADDING + LABEL_HEIGHT;
        inpRect.y = btnRect.y;

        GuiLabel(btnRect, "Author");

        if (GuiTextBox(
                inpRect, state->author, MAX_GLYPH_NAME_LEN,
                state->authorEditMode
            )) {
            state->authorEditMode = !state->authorEditMode;
        }

        btnRect.y += PADDING + LABEL_HEIGHT;
        inpRect.y = btnRect.y;

        GuiLabel(btnRect, "License");

        if (GuiTextBox(
                inpRect, state->license, MAX_GLYPH_NAME_LEN,
                state->licenseEditMode
            )) {
            state->licenseEditMode = !state->licenseEditMode;
        }

        btnRect.y += PADDING + LABEL_HEIGHT;
        inpRect.y = btnRect.y;

        GuiLabel(btnRect, "Description");

        if (GuiTextBox(
                inpRect, state->description, MAX_GLYPH_DESC_LEN,
                state->descEditMode
            )) {
            state->descEditMode = !state->descEditMode;
        }

        btnRect.width = 80;
        btnRect.y += INFO_W_MARGIN * 2 + LABEL_HEIGHT;

        bool saveClicked = GuiButton(btnRect, "Save");
        btnRect.x += PADDING + btnRect.width;
        bool cancelClicked = GuiButton(btnRect, "Cancel");

        if (saveClicked) {
            updateGlyphInfo(state, obj);
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

#include "include/itemselector.h"
#include "include/ext/raygui.h"
#include "include/ext/raylib.h"
#include <stdbool.h>
#include <string.h>

#define NEW_BTN_ID  0
#define EDIT_BTN_ID 1

ItemSelectorState CreateItemSelector(int width, int topSkip, int bottomSkip) {
    return (ItemSelectorState){
        .enabled = true,
        .width = width,
        .topSkip = topSkip + ITEM_PANEL_MARGIN,
        .bottomSkip = bottomSkip + ITEM_PANEL_MARGIN,
        .lastActive = 0,
        .lastFocus = 0,
        .active = 0,
        .index = 0,
        .focus = 0,
        .newBtnClicked = false,
        .editBtnClicked = false,
    };
}

void itemCtrlBtns(ItemSelectorState *state, float x, float y) {
    int maxWidth = state->width - ITEM_PANEL_PADDING * 2;
    float widthForNewBtn = maxWidth * (2.0 / 3.0) - 5;
    // TODO: MARGIN -> 5;

    state->newBtnClicked = GuiButton(
        (Rectangle){
            x,
            y,
            widthForNewBtn,
            ITEM_PANEL_CTRL_BTN_HEIGHT,
        },
        GuiIconText(ICON_KEY, "new")
    );

    state->editBtnClicked = GuiButton(
        (Rectangle){
            x + widthForNewBtn + ITEM_PANEL_CTRL_BTN_PADDING,
            y,
            maxWidth - widthForNewBtn - ITEM_PANEL_CTRL_BTN_PADDING,
            ITEM_PANEL_CTRL_BTN_HEIGHT,
        },
        GuiIconText(ICON_PENCIL_BIG, NULL)
    );
}

void ItemSelector(ItemSelectorState *state, const char **names, int len) {

    Rectangle itemPanelBounds =
        (Rectangle){ITEM_PANEL_MARGIN, state->topSkip, state->width,
                    GetScreenHeight() - state->topSkip - state->bottomSkip};

    GuiGroupBox(itemPanelBounds, "~items");
    itemCtrlBtns(
        state, itemPanelBounds.x + ITEM_PANEL_PADDING,
        itemPanelBounds.y + ITEM_PANEL_PADDING
    );

    int listY = itemPanelBounds.y + ITEM_PANEL_CTRL_BTN_HEIGHT +
                (ITEM_PANEL_PADDING * 2);

    state->lastActive = state->active;
    GuiListViewEx(
        (Rectangle){itemPanelBounds.x + ITEM_PANEL_PADDING, listY,
                    state->width - (ITEM_PANEL_PADDING * 2),
                    itemPanelBounds.height - ITEM_PANEL_CTRL_BTN_HEIGHT -
                        (ITEM_PANEL_PADDING * 3)},
        names, len, &state->index, &state->active, &state->focus

    );

    if (state->active < 0) {
        state->active = state->lastActive;
    }

    if (state->focus < 0) {
        state->focus = state->active;
    }
}

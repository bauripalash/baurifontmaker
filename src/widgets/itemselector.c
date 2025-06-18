#include "../include/widgets/itemselector.h"
#include "../include/colors.h"
#include "../include/ext/raygui.h"
#include "../include/ext/raylib.h"
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

#define MARGIN_BTN    5
#define CELL_SIZE     6
#define ToolTipBg     (Fade(ColorGrayDarkest, 0.8))
#define ToolTipBorder (Fade(ColorGrayDarkest, 0.9))
#define ToolTipFill   (Fade(ColorPrimary, 1))

void itemCtrlBtns(ItemSelectorState *state, float x, float y) {
    int maxWidth = state->width - ITEM_PANEL_PADDING * 2;
    float widthForNewBtn = maxWidth * (2.0 / 3.0) - MARGIN_BTN;

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

void ItemSelector(ItemSelectorState *state, FontItemList *list) {

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
    int result = GuiListViewEx(
        (Rectangle){itemPanelBounds.x + ITEM_PANEL_PADDING, listY,
                    state->width - (ITEM_PANEL_PADDING * 2),
                    itemPanelBounds.height - ITEM_PANEL_CTRL_BTN_HEIGHT -
                        (ITEM_PANEL_PADDING * 3)},
        list->names, list->len, &state->index, &state->active, &state->focus

    );

    if (state->active < 0) {
        state->active = state->lastActive;
    }

    int rawFocus = state->focus;
    if (rawFocus != -1 && rawFocus != state->active) {
        FontItem *currentItem = list->items[rawFocus];
        Vector2 mousePos = GetMousePosition();
        Rectangle hoverRect = {
            mousePos.x + 10, mousePos.y + 10, CELL_SIZE * 10, CELL_SIZE * 10
        };

        DrawRectangleRec(hoverRect, ToolTipBg);
        DrawRectangleLinesEx(hoverRect, 2, ToolTipBorder);

        int posx = hoverRect.x + CELL_SIZE;
        int posy = hoverRect.y + CELL_SIZE;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Color clr = FontItemGetFlip(currentItem, col, row) ? ToolTipFill
                                                                   : ToolTipBg;

                DrawRectangleRec(
                    (Rectangle){posx, posy, CELL_SIZE, CELL_SIZE}, clr
                );

                posx += CELL_SIZE;
            }
            posy += CELL_SIZE;
            posx = hoverRect.x + CELL_SIZE;
        }
    }

    if (state->focus < 0) {
        state->focus = state->active;
    }
}

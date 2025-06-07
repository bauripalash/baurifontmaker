#ifndef ITEM_SELECTOR_H
#define ITEM_SELECTOR_H
#include <stdbool.h>

#define ITEM_PANEL_PADDING          10
#define ITEM_PANEL_MARGIN           20
#define ITEM_PANEL_CTRL_BTN_HEIGHT  40
#define ITEM_PANEL_CTRL_BTN_PADDING 5

typedef struct {
    bool enabled;

    int topSkip;
    int bottomSkip;
    int width;

    int lastActive;
    int lastFocus;
    int active;
    int index;
    int focus;

    bool newBtnClicked;
    bool editBtnClicked;

} ItemSelectorState;

ItemSelectorState CreateItemSelector(int width, int topSkip, int bottomSkip);
void ItemSelector(ItemSelectorState *state, const char **names, int len);

#endif

#ifndef _GUI_H_
#define _GUI_H_

#include "ext/mlib/m-dict.h"
#include "ext/raylib.h"
#include "fontitem.h"
#include "fontitemlist.h"
#include "itemselector.h"
#include "toolbar.h"
#include "windows/edititem.h"
#include "windows/newitem.h"

#include <stdbool.h>
#include <stdint.h>

DICT_SET_DEF(flipset, uint8_t)

typedef struct {
    int gridBtnSize;
    bool enableGrid;
    int gridThickness;
    Vector2 gridSize;

    float toolbarHeight;
    float statusbarHeight;

    float itemListWidth;
    bool isPopupActive;

    ToolbarState toolbarState;

    ItemSelectorState itemSelectorState;

    NewItemWindowState newItemWindowState;
    EditItemWindowState editItemWindowState;

} UiConfig;

typedef struct Gui {
    int winWidth;
    int winHeight;
    const char *title;
    UiConfig *conf;
    flipset_t flipped;
    Vector2 itemListAnchor;
    FontItemList *items;
    FontItem *currentItem;

} Gui;

Gui *NewGUI();
void FreeGui(Gui *ui);
void GuiMain();

#endif

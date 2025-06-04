#ifndef _GUI_H_
#define _GUI_H_

#include "ext/mlib/m-dict.h"
#include "ext/mlib/m-string.h"
#include "ext/raylib.h"
#include "fontvalue.h"
#include "strlist.h"

#include <stdbool.h>
#include <stdint.h>

DICT_SET_DEF(flipset, uint8_t)
DICT_SET_DEF(itemlist, string_t, STRING_OPLIST)

typedef struct Gui {
    int winWidth;
    int winHeight;
    const char *title;
    flipset_t flipped;
    FontValue *value;
    int btnSize;
    bool enableGrid;
    int gridThickness;
    Vector2 gridSize;
    itemlist_t items;
    float toolbarHeight;
    float statusbarHeight;
    Vector2 canvasAnchor;
    Vector2 itemListAnchor;
    float itemListWidth;
    int itemListIndex;
    int itemListActive;
    StrList *listItems;

} Gui;

Gui *NewGUI();
void FreeGui(Gui *ui);
void GuiMain();

#endif

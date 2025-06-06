#ifndef WIN_NEWITEM_H
#define WIN_NEWITEM_H

#include "../ext/raygui.h"
#include "../ext/raylib.h"

#include <stdbool.h>

#define NW_MARGIN 30
#define NW_WIDTH  500
#define NW_HEIGHT 200

typedef struct {
    bool windowActive;
    Rectangle windowBounds;
    char hexCode[128];
    char itemName[128];

} NewItemWindowState;

NewItemWindowState CreateNewItemWindow(void);
bool NewItemWindow(NewItemWindowState *state);
#endif

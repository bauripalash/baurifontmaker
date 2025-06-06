#ifndef WIN_EDITITEM_H
#define WIN_EDITITEM_H

#include "../ext/raygui.h"
#include "../ext/raylib.h"
#include "../fontitem.h"

#include <stdbool.h>

#define EDIT_W_MARGIN 30
#define EDIT_W_WIDTH  500
#define EDIT_W_HEIGHT 200

typedef struct {
    bool windowActive;
    Rectangle windowBounds;
    char hexCode[128];

} EditItemWindowState;

EditItemWindowState CreateEditItemWindow(void);
bool EditItemWindow(EditItemWindowState *state, FontItem *targetItem);
#endif

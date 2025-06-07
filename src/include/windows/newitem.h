#ifndef WIN_NEWITEM_H
#define WIN_NEWITEM_H

#include "../ext/raygui.h"
#include "../ext/raylib.h"

#include <stdbool.h>

#define NW_MARGIN     30
#define NW_WIDTH      500
#define NW_HEIGHT     200

#define NW_ERR_WIDTH  400
#define NW_ERR_HEIGHT 100

#define MAX_HEXCODE   16
#define MAX_NAME      128

typedef struct {
    bool windowActive;
    Rectangle windowBounds;
    int errWinX;
    int errWinY;
    char hexStr[MAX_HEXCODE + 1];
    char nameStr[MAX_NAME + 1];
    int hexValue;
    bool hexEditMode;
    bool nameEditMode;

    bool invalidHexError;

} NewItemWindowState;

NewItemWindowState CreateNewItemWindow(void);
void CleanNewItemState(NewItemWindowState *state);
bool NewItemWindow(NewItemWindowState *state);
#endif

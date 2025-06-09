#ifndef WIN_EDITITEM_H
#define WIN_EDITITEM_H

#include "../ext/raylib.h"
#include "../fontitem.h"

#include <stdbool.h>

#define EDIT_W_MARGIN     30
#define EDIT_W_WIDTH      500
#define EDIT_W_HEIGHT     300

#define EDIT_SAVE_CLICK   0
#define EDIT_CANCEL_CLICK 1
#define EDIT_REMOVE_CLICK 2

#define EDIT_ERR_WIDTH    400
#define EDIT_ERR_HEIGHT   100

// TODO: Make error window sizes common

typedef struct {
    bool windowActive;
    Rectangle windowBounds;

    int errWinX;
    int errWinY;

    char hexStr[17];
    char nameStr[129];
    char indexStr[11];
    int indexValue;

    bool hexEditMode;
    bool nameEditMode;
    bool indexEditMode;

    bool invalidHexError;
    bool invalidIndexError;

    bool saveClicked;
    bool cancelClicked;
    bool removeClicked;

} EditItemState;

EditItemState CreateEditItemState(void);
void SetStateEditItem(EditItemState *state, FontItem *target, int itemLen);
int EditItemWindow(EditItemState *state, FontItem *targetItem, int itemsLen);
#endif

#ifndef INFO_EDIT_H
#define INFO_EDIT_H
#include "../../external/raylib.h"
#include "../glyph.h"

#include <stdbool.h>
#define INFO_W_MARGIN 30
#define INFO_W_WIDTH  500
#define INFO_W_HEIGHT 300

typedef struct {
    bool windowActive;
    Rectangle windowBounds;

    char name[MAX_GLYPH_NAME_LEN];
    char author[MAX_GLYPH_NAME_LEN];
    char license[MAX_GLYPH_NAME_LEN];
    char description[MAX_GLYPH_DESC_LEN];

    bool nameEditMode;
    bool authorEditMode;
    bool licenseEditMode;
    bool descEditMode;

} InfoEditState;

InfoEditState CreateInfoEditState();
bool InfoEditWindow(InfoEditState *state, GlyphObj *obj);
void UpdateInfo(InfoEditState *state, const GlyphObj *obj);

#endif

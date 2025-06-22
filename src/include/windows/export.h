#ifndef EXPORT_H
#define EXPORT_H
#include "../../external/raylib.h"
#include "../glyph.h"
#include <stdbool.h>
#define EXPORT_W_WIDTH     700
#define EXPORT_W_HEIGHT    500
#define EXPORT_W_MARGIN    30
#define EXPORT_BUFFER_SIZE 1024 * 1024

#define TYPE_SEL_STR       "Code;Font"
#define TYPE_SEL_CODE      0
#define TYPE_SEL_FONT      1

#define LANG_SEL_STR       "C Header;Python;Rust<TODO>"
#define LANG_SEL_CHEADER   0
#define LANG_SEL_PYTHON    1
#define LANG_SEL_RUST      2

#define FONT_SEL_STR       "BDF;TTF<TODO>;OTF<TODO>"
#define FONT_SEL_BDF       0
#define FONT_SEL_TTF       1
#define FONT_SEL_OTF       2

typedef struct {
    bool windowActive;
    Rectangle windowBounds;
    char *buffer;
    bool saveBtnClicked;
    bool copyBtnClicked;
    Vector2 scrollOffset;
    Rectangle scrollView;
    Font font;
    int codeHeight;
    int typeSelectActive;
    int typeSelectEdit;
    int langSelectActive;
    int langSelectEdit;
    int fontTypeActive;
    int fontTypeEdit;

} ExportState;

ExportState CreateExportState();
bool ExportWindow(ExportState *state, GlyphObj *obj);
void ClearExportBuffer(ExportState *state);

#endif

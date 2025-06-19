#ifndef EXPORT_H
#define EXPORT_H
#include "../ext/raylib.h"
#include "../objects/glyph.h"
#include <stdbool.h>
#define EXPORT_W_WIDTH     700
#define EXPORT_W_HEIGHT    500
#define EXPORT_W_MARGIN    30
#define EXPORT_BUFFER_SIZE 1024 * 1024

typedef struct {
    bool windowActive;
    Rectangle windowBounds;
    char *buffer;
    bool codeBtnClicked;

} ExportState;

ExportState CreateExportState();
bool ExportWindow(ExportState *state, GlyphObj *obj);
void ClearExportBuffer(ExportState *state);

#endif

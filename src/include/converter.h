#ifndef CONVERTER_H
#define CONVERTER_H

#include "glyph.h"
#include <stdbool.h>
#define MAX_BDF_FONT_SIZE 512 * 512
bool ExportToBDF(GlyphObj *g, const char *filename);
char *GenerateBDFFont(const GlyphObj *g);

#endif

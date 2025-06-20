#ifndef CONVERTER_H
#define CONVERTER_H

#include "glyph.h"
#include <stdbool.h>

bool ExportToBDF(GlyphObj *g, const char *filename);

#endif

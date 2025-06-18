#ifndef CONVERT_BDF_H
#define CONVERT_BDF_H

#include "../objects/glyph.h"
#include <stdbool.h>

bool ExportToBDF(GlyphObj *g, const char *filename);

#endif

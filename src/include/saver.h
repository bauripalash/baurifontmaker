#ifndef SAVER_H
#define SAVER_H

#include "objects/glyph.h"
#include <stdbool.h>
bool SaveFontFileAsBfont(const GlyphObj *g, const char *filename);

#endif

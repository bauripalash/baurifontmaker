#ifndef CODEGEN_H
#define CODEGEN_H

#include "objects/glyph.h"
#include <stdbool.h>
char *GenerateCHeader(const GlyphObj *glyph);

#endif

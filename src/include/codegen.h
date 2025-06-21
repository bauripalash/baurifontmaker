#ifndef CODEGEN_H
#define CODEGEN_H

#include "glyph.h"
#include <stdbool.h>

char *GetFilledHeader(const char *template, const GlyphObj *glyph);
char *GenerateCHeader(const GlyphObj *glyph);
char *GeneratePython(const GlyphObj *glyph);
char *GenerateRust(const GlyphObj *glyph);

#define MAX_CODEGEN_SIZE 1048576

static const char *C_TEMPLATE = "\
/*******************************\n\
* Glyphs Generated With BauriGlyphMaker v${VERSION} \n\
* Name : ${NAME}\n\
* Author : ${AUTHOR}\n\
* License : ${LICENSE}\n\
* Description: ${DESCRIPTION}\n\
********************************/\n\
\n\
${CODE}\n\
";

static const char *PYTHON_TEMPLATE = "\
################################\n\
# Glyphs Generated With BauriGlyphMaker v${VERSION} \n\
# Name : ${NAME}\n\
# Author : ${AUTHOR}\n\
# License : ${LICENSE}\n\
# Description: ${DESCRIPTION}\n\
################################\n\
\n\
${CODE}\n\
";

#endif

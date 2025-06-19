#include "../include/codegen.h"
#include "../include/ext/raylib.h"
#include <stdbool.h>
#include <stdlib.h>

static const char *template = "\
/*******************************\n\
*\n\
* Glyphs Generated With BauriGlyphMaker v${VERSION} \n\
* Name : ${NAME}\n\
* Author : ${AUTHOR}\n\
* License : ${LICENSE}\n\
* Description: ${DESCRIPTION}\n\
********************************/\n\
${CODE}\n\
";

char *GenerateCHeader(const GlyphObj *glyph) {
    TraceLog(LOG_WARNING, "CODE %s", template);
    return TextReplace(template, "${NAME}", glyph->name);
}

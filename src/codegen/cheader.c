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

char *getCode(const GlyphObj *obj);

char *GenerateCHeader(const GlyphObj *glyph) {
    char *name = TextReplace(template, "${NAME}", glyph->name);
    char *author = TextReplace(name, "${AUTHOR}", glyph->author);
    MemFree(name);
    char *license = TextReplace(author, "${LICENSE}", glyph->license);
    MemFree(author);
    char *desc = TextReplace(license, "${DESCRIPTION}", glyph->description);
    MemFree(license);
    char *src = getCode(glyph);
    char *code = TextReplace(desc, "${CODE}", src);
    // Free src -> bfree(src)
    MemFree(desc);
    return code;
}

char *getCode(const GlyphObj *obj) { return "<TODO>"; }

#include "../include/codegen.h"
#include "../external/raylib.h"

char *GetFilledHeader(const char *template, const GlyphObj *glyph) {
    char *name = TextReplace(template, "${NAME}", glyph->name);
    char *author = TextReplace(name, "${AUTHOR}", glyph->author);
    MemFree(name);
    char *license = TextReplace(author, "${LICENSE}", glyph->license);
    MemFree(author);
    char *desc = TextReplace(license, "${DESCRIPTION}", glyph->description);
    MemFree(license);

    return desc;
}

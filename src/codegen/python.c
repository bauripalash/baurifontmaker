#include "../external/raylib.h"
#include "../include/balloc.h"
#include "../include/codegen.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static bool getCode(const GlyphObj *obj, char *buffer);
char *GeneratePython(const GlyphObj *glyph) {
    char *header = GetFilledHeader(PYTHON_TEMPLATE, glyph);

    char *codeBuffer = (char *)balloc(MAX_CODEGEN_SIZE * sizeof(char));
    char *src = NULL;

    if (codeBuffer == NULL) {
        src = "<[ERROR] Failed to allocate memory for Python code>";
    } else {

        if (!getCode(glyph, codeBuffer)) {
            src = "<[ERROR] Failed to generate Python code>";
        } else {
            src = codeBuffer;
        }
    }
    char *code = TextReplace(header, "${CODE}", src);

    if (codeBuffer != NULL) {
        bfree(codeBuffer);
    }
    MemFree(header);
    return code;
}

bool getCode(const GlyphObj *glyph, char *buffer) {

    int pos = 0;
    char *nameUpper = TextToUpper(glyph->name);
    TextAppend(
        buffer, TextFormat("%s_GLYPH_HEIGHT = %d\n", nameUpper, -100), &pos
    );
    TextAppend(
        buffer, TextFormat("%s_GLYPH_WIDTH = %d\n", nameUpper, -100), &pos
    );
    int count = glyph->count;
    TextAppend(
        buffer, TextFormat("%s_GLYPH_COUNT = %d\n\n", nameUpper, count), &pos
    );
    TextAppend(buffer, TextFormat("%s_CODEPOINTS = [\n", nameUpper), &pos);

    for (int i = 0; i < count; i++) {
        int codepoint = glyph->glyphs[i]->value;
        char *name = glyph->glyphs[i]->name;
        TextAppend(
            buffer, TextFormat("    0x%X,    #%s\n", codepoint, name), &pos
        );
    }

    TextAppend(buffer, TextFormat("]\n\n%s_GLYPH_DATA = [\n", nameUpper), &pos);

    for (int i = 0; i < count; i++) {
        char *name = glyph->glyphs[i]->name;
        uint8_t *code = glyph->glyphs[i]->bits;
        TextAppend(buffer, "    [", &pos);
        for (int j = 0; j < 8; j++) {
            TextAppend(buffer, TextFormat("0x%X, ", code[j]), &pos);
        }
        TextAppend(buffer, TextFormat("],    #%s\n", name), &pos);
    }
    TextAppend(buffer, "]\n", &pos);

    return true;
}

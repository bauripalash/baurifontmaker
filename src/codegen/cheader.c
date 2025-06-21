#include "../external/raylib.h"
#include "../include/balloc.h"
#include "../include/codegen.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static bool getCode(const GlyphObj *obj, char *buffer);

char *GenerateCHeader(const GlyphObj *glyph) {

    char *header = GetFilledHeader(C_TEMPLATE, glyph);

    char *codeBuffer = (char *)balloc(MAX_CODEGEN_SIZE * sizeof(char));
    char *src = NULL;

    if (codeBuffer == NULL) {
        src = "<[ERROR] Failed to allocate memory for C Header>";
    } else {

        if (!getCode(glyph, codeBuffer)) {
            src = "<[ERROR] Failed to generate C Header>";
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

bool getCode(const GlyphObj *obj, char *buffer) {

    int pos = 0;
    char *nameUpper = TextToUpper(obj->name);
    TextAppend(
        buffer, TextFormat("#define %s_GLYPH_HEIGHT %d\n", nameUpper, -100),
        &pos
    );

    TextAppend(
        buffer, TextFormat("#define %s_GLYPH_WIDTH %d\n", nameUpper, -100), &pos
    );

    int count = obj->count;
    TextAppend(
        buffer, TextFormat("#define %s_GLYPH_COUNT %d\n\n", nameUpper, count),
        &pos
    );
    TextAppend(
        buffer,
        TextFormat(
            "const int %s_CODEPOINTS[%s_GLYPH_COUNT] = {\n", nameUpper,
            nameUpper
        ),
        &pos
    );
    for (int i = 0; i < count; i++) {
        int codepoint = obj->glyphs[i]->value;
        char *name = obj->glyphs[i]->name;
        TextAppend(
            buffer, TextFormat("    0x%X,    //%s\n", codepoint, name), &pos
        );
    }

    TextAppend(
        buffer,
        TextFormat(
            "};\n\nconst int %s_GLYPH_DATA[%s_GLYPH_COUNT][7]= {\n", nameUpper,
            nameUpper
        ),
        &pos
    );

    for (int i = 0; i < count; i++) {
        char *name = obj->glyphs[i]->name;
        uint8_t *code = obj->glyphs[i]->bits;
        TextAppend(buffer, "    {", &pos);
        for (int j = 0; j < 8; j++) {
            TextAppend(buffer, TextFormat("0x%X, ", code[j]), &pos);
        }
        TextAppend(buffer, TextFormat("},    //%s\n", name), &pos);
    }
    TextAppend(buffer, "};\n", &pos);

    return true;
}

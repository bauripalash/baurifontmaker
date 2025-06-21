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
    TextAppend(buffer, "# NOTE: Python Code Generation Not Available", &pos);
    return true;
}

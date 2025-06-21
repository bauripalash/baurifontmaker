#include "../external/raylib.h"
#include "../include/balloc.h"
#include "../include/codegen.h"

#include <stdbool.h>
#include <stdio.h>

// 1024 * 1024
#define MAX_CODE_SIZE 1048576

static const char *template = "\
/*******************************\n\
*\n\
* Glyphs Generated With BauriGlyphMaker v${VERSION} \n\
* Name : ${NAME}\n\
* Author : ${AUTHOR}\n\
* License : ${LICENSE}\n\
* Description: ${DESCRIPTION}\n\
********************************/\n\
\n\
${CODE}\n\
";

bool getCode(const GlyphObj *obj, char *buffer);

char *GenerateCHeader(const GlyphObj *glyph) {
    char *name = TextReplace(template, "${NAME}", glyph->name);
    char *author = TextReplace(name, "${AUTHOR}", glyph->author);
    MemFree(name);
    char *license = TextReplace(author, "${LICENSE}", glyph->license);
    MemFree(author);
    char *desc = TextReplace(license, "${DESCRIPTION}", glyph->description);
    MemFree(license);

    char *codeBuffer = (char *)balloc(MAX_CODE_SIZE * sizeof(char));
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
    printf("CODE -> \n%s\n", src);
    char *code = TextReplace(desc, "${CODE}", src);

    if (codeBuffer != NULL) {
        bfree(codeBuffer);
    }
    MemFree(desc);
    return code;
}

bool getCode(const GlyphObj *obj, char *buffer) {

    int pos = 0;
    char *nameUpper = TextToUpper(obj->name);
    TextAppend(
        buffer, TextFormat("#define %s_HEIGHT %d\n", nameUpper, -100), &pos
    );

    TextAppend(
        buffer, TextFormat("#define %s_WIDTH %d\n", nameUpper, -100), &pos
    );

    TextAppend(
        buffer,
        TextFormat("const int[%d] %s_CODEPOINTS = {\n", obj->count, nameUpper),
        &pos
    );
    for (int i = 0; i < obj->count; i++) {
        int codepoint = obj->glyphs[i]->value;
        char *name = obj->glyphs[i]->name;
        TextAppend(
            buffer, TextFormat("    0x%X, //%s\n", codepoint, name), &pos
        );
    }

    TextAppend(buffer, "};", &pos);

    return true;
}

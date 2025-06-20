#include "include/glyph.h"
#include "external/raygui.h"
#include "external/raylib.h"
#include "include/balloc.h"
#include "include/glyphitem.h"

#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

GlyphObj *NewGlyph(const char *name) {
    GlyphObj *g = (GlyphObj *)balloc(sizeof(GlyphObj));
    if (g == NULL) {
        TraceLog(LOG_ERROR, "Failed to create new glyph object");
        return NULL;
    }

    if (name != NULL) {
        strcpy(g->name, name);
    } else {
        strcpy(g->name, "Untitled");
    }

    strcpy(g->author, "<AUTHOR>");
    strcpy(g->license, "<LICENSE>");
    strcpy(g->description, "New Glyph Object");
    g->count = 0;

    g->glyphs = NULL;
    g->gnames = NULL;

    return g;
}

void GlyphSetName(GlyphObj *g, const char *name) { strcpy(g->name, name); }
void GlyphSetAuthor(GlyphObj *g, const char *author) {
    strcpy(g->author, author);
}
void GlyphSetLicense(GlyphObj *g, const char *license) {
    strcpy(g->license, license);
}
void GlyphSetDescription(GlyphObj *g, const char *desc) {
    strcpy(g->description, desc);
}

void freeList(GlyphObj *gl) {
    int count = arrlen(gl->gnames);
    for (int i = count; i > 0; i--) {
        arrpop(gl->gnames);
        GlyphItem *item = arrpop(gl->glyphs);
        FreeGlyphItem(item);
    }

    arrfree(gl->gnames);
    arrfree(gl->glyphs);
}

void FreeGlyph(GlyphObj *glyph) {
    if (glyph == NULL) {
        return;
    }

    if (glyph->glyphs != NULL) {
        freeList(glyph);
    }

    bfree(glyph);
}

bool AddNewGlyphItem(GlyphObj *glyph, const char *name, int value) {
    if (glyph == NULL || name == NULL) {
    }
    GlyphItem *tempItem = NewGlyphItem(name);
    if (tempItem == NULL) {
        return false;
    }

    SetGlyphValue(tempItem, value);

    tempItem->listIndex = glyph->count;
    arrput(glyph->glyphs, tempItem);
    arrput(glyph->gnames, tempItem->name); // TODO: check errors

    glyph->count = arrlen(glyph->glyphs);
    return true;
}

int GetIndexOfGlyph(GlyphObj *glyph, int value) {
    int len = arrlen(glyph->glyphs);
    for (int i = 0; i < len; i++) {
        if (glyph->glyphs[i]->value == value) {
            return i;
        }
    }

    return -1;
}

int GetIndexOfGlyphByName(GlyphObj *glyph, const char *name) {
    int len = arrlen(glyph->glyphs);

    for (int i = 0; i < len; i++) {
        const char *name = glyph->glyphs[i]->name;
        if (!TextIsEqual(name, name)) {
            return i;
        }
    }

    return -1;
}

bool ItemExists(GlyphObj *glyph, int value) {
    return GetIndexOfGlyph(glyph, value) != -1;
}

bool ItemExistsName(GlyphObj *glyph, const char *name) {
    return GetIndexOfGlyphByName(glyph, name) != -1;
}

// TODO: return error codes;
void MoveGlyphItem(GlyphObj *glyph, int from, int to) {
    if (glyph == NULL || glyph->gnames == NULL || glyph->glyphs == NULL) {
        return;
    }

    int len = arrlen(glyph->gnames);

    if (from == to) {
        return;
    }

    if (from < 0 || from >= len) {
        return;
    }

    if (to < 0 || to >= len) {
        return;
    }

    GlyphItem *item = glyph->glyphs[from];
    item->listIndex = to;

    arrdel(glyph->glyphs, from);
    arrdel(glyph->gnames, from);

    arrins(glyph->glyphs, to, item);
    arrins(glyph->gnames, to, item->name);

    // get the 'toppest' item,
    // if the list is imagined as a stack type thing
    int startIndex = from < to ? from : to;

    for (int i = startIndex; i < len; i++) {
        glyph->glyphs[i]->listIndex = i;
    }
}

void RenameGlyphItem(GlyphObj *glyph, int index, const char *name) {
    if (glyph == NULL || glyph->gnames == NULL || glyph->glyphs == NULL) {
        return; // TODO: error message
    }

    int len = arrlen(glyph->gnames);

    if (index < 0 || index >= len) {
        return; // TODO: error msg
    }

    GlyphItem *item = glyph->glyphs[index];
    arrdel(glyph->gnames, index);
    SetGlyphItemName(item, name);
    arrins(glyph->gnames, index, item->name);
}

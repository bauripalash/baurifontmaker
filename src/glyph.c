#include "include/glyph.h"
#include "external/raygui.h"
#include "include/balloc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

GlyphItemList *newItemList();
void freeItemList(GlyphItemList *gl);
bool addToItemList(GlyphItemList *gl, GlyphItem *item);

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

    GlyphItemList *list = newItemList();

    if (list == NULL) {
        TraceLog(LOG_ERROR, "Failed to create item list for glyph object");
        return false;
    }

    g->glyphs = list;

    return g;
}
void FreeGlyph(GlyphObj *glyph) {
    if (glyph == NULL) {
        return;
    }

    if (glyph->glyphs != NULL) {
        freeItemList(glyph->glyphs);
    }

    bfree(glyph);
}

bool AddNewGlyphItem(GlyphObj *glyph, const char *name, int value) {
    GlyphItem *tempItem = NewGlyphItem(name);
    if (tempItem == NULL) {
        return false;
    }

    SetGlyphValue(tempItem, value);
    bool addOk = addToItemList(glyph->glyphs, tempItem);

    if (!addOk) {
        FreeGlyphItem(tempItem);
        return false;
    }

    glyph->count = glyph->glyphs->len;
    return true;
}
void GlyphSetName(GlyphObj *g, const char *name);
void GlyphSetAuthor(GlyphObj *g, const char *author);
void GlyphSetLicense(GlyphObj *g, const char *license);
void GlyphSetDescription(GlyphObj *g, const char *desc);

GlyphItemList *newItemList() {
    GlyphItemList *gl = (GlyphItemList *)balloc(sizeof(GlyphItemList));
    if (gl == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate glyph item list");
        return NULL;
    }

    gl->cap = 10;
    gl->len = 0;

    gl->items = balloc(gl->cap * sizeof(GlyphItem *));

    if (gl->items == NULL) {
        TraceLog(
            LOG_ERROR, "Failed to allocate memory for glyph item list array"
        );
        bfree(gl);
        return NULL;
    }

    gl->names = balloc(gl->cap * sizeof(char **));

    if (gl->names == NULL) {
        TraceLog(
            LOG_ERROR, "Failed to allocate memory for glyph item list names"
        );
        bfree(gl->items);
        bfree(gl);
        return NULL;
    }

    return gl;
}

void freeItemList(GlyphItemList *gl) {
    if (gl == NULL) {
        return;
    }
    for (size_t i = 0; i < gl->len; i++) {
        FreeGlyphItem(gl->items[i]);
    }

    bfree(gl->items);
    bfree(gl->names);
    bfree(gl);
}

bool resizeItemList(GlyphItemList *fl) {
    if (fl->len >= fl->cap) {
        fl->cap *= 2;
        GlyphItem **newItems =
            brealloc(fl->items, fl->cap * sizeof(GlyphItem *));
        if (newItems == NULL) {
            TraceLog(LOG_ERROR, "Failed to increase size of glyph item list");
            return false;
        }
        const char **newNames =
            brealloc(fl->names, fl->cap * sizeof(const char *));
        if (newNames == NULL) {
            TraceLog(
                LOG_ERROR, "Failed to increase size of glyph item name list"
            );
            return false;
        }
        fl->items = newItems;
        fl->names = newNames;
    }

    return true;
}

bool addToItemList(GlyphItemList *gl, GlyphItem *item) {
    if (!resizeItemList(gl)) {
        return false;
    }

    gl->items[gl->len] = item;
    gl->names[gl->len] = item->name;
    item->listIndex = gl->len;
    gl->len++;

    return true;
}

void moveItemList(GlyphItemList *gl, size_t from, size_t to) {
    if (from >= gl->len || to >= gl->len) {
        TraceLog(LOG_ERROR, "Invalid index");
        return;
    }

    if (from == to) {
        return;
    }

    GlyphItem *tempItem = gl->items[from];
    const char *tempName = gl->names[from];

    if (from < to) {
        memmove(
            &gl->items[from], &gl->items[from + 1],
            (to - from) * sizeof(GlyphItem *)
        );
        memmove(
            &gl->names[from], &gl->items[from + 1],
            (to - from) * sizeof(GlyphItem *)
        );
    } else { // from > to
        memmove(
            &gl->items[to + 1], &gl->items[to],
            (from - to) * sizeof(GlyphItem *)
        );
        memmove(
            &gl->names[to + 1], &gl->names[to],
            (from - to) * sizeof(const char *)
        );
    }

    tempItem->listIndex = to;
    gl->items[to] = tempItem;
    gl->names[to] = tempName;
}

#ifndef GLYPH_H
#define GLYPH_H

#include "glyphitem.h"

#define MAX_GLYPH_NAME_LEN 256
#define MAX_GLYPH_DESC_LEN 512

typedef struct {
    GlyphItem **items;
    const char **names;
    size_t len;
    size_t cap;
} GlyphItemList;

typedef struct {
    char name[MAX_GLYPH_NAME_LEN];
    char author[MAX_GLYPH_NAME_LEN];
    char license[MAX_GLYPH_NAME_LEN];
    char description[MAX_GLYPH_DESC_LEN];

    GlyphItemList *glyphs;
    int count;
} GlyphObj;

GlyphObj *NewGlyph(const char *name);
void FreeGlyph(GlyphObj *glyph);
bool AddNewGlyphItem(GlyphObj *glyph, const char *name, int value);
void GlyphSetName(GlyphObj *g, const char *name);
void GlyphSetAuthor(GlyphObj *g, const char *author);
void GlyphSetLicense(GlyphObj *g, const char *license);
void GlyphSetDescription(GlyphObj *g, const char *desc);

#endif

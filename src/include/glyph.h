#ifndef GLYPH_H
#define GLYPH_H

#include "glyphitem.h"

#define MAX_GLYPH_NAME_LEN 256
#define MAX_GLYPH_DESC_LEN 512

typedef struct {
    char name[MAX_GLYPH_NAME_LEN];
    char author[MAX_GLYPH_NAME_LEN];
    char license[MAX_GLYPH_NAME_LEN];
    char description[MAX_GLYPH_DESC_LEN];

    GlyphItem **glyphs;
    const char **gnames;
    int count;
} GlyphObj;

GlyphObj *NewGlyph(const char *name);
void FreeGlyph(GlyphObj *glyph);
bool AddNewGlyphItem(GlyphObj *glyph, const char *name, int value);

void GlyphSetName(GlyphObj *g, const char *name);
void GlyphSetAuthor(GlyphObj *g, const char *author);
void GlyphSetLicense(GlyphObj *g, const char *license);
void GlyphSetDescription(GlyphObj *g, const char *desc);

int GetIndexOfGlyph(GlyphObj *glyph, int value);
int GetIndexOfGlyphByName(GlyphObj *glyph, const char *name);
bool ItemExistsName(GlyphObj *glyph, const char *name);
bool ItemExists(GlyphObj *glyph, int value);
void MoveGlyphItem(GlyphObj *glyph, int from, int to);
void RenameGlyphItem(GlyphObj *glyph, int index, const char *name);

#endif

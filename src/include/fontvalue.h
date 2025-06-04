#ifndef _FONTVALUE_H
#define _FONTVALUE_H

#include <stdbool.h>
#include <stdint.h>
typedef struct FontValue {
    uint8_t bits[8];
} FontValue;

FontValue *NewFontValue();
void FreeFontValue(FontValue *fv);
uint8_t FlipFontValue(FontValue *fv, uint8_t col, uint8_t row);
bool GetBool(FontValue *fv, uint8_t col, uint8_t row);
void LogFontValue(FontValue *fv);

#endif

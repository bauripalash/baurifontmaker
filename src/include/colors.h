#ifndef _COLORS_H_
#define _COLORS_H_

#include "ext/raylib.h"

// Penginette18 Palette
// Source: https://lospec.com/palette-list/penginette18

#define HexColorPrimary      (int)0x021a40ff
#define HexColorGrayDarkest  (int)0x475669ff
#define HexColorGrayLighter  (int)0x829e9fff
#define HexColorGrayLightest (int)0xb1c1bbff
#define HexColorBackground   (int)0xfffaddff
#define HexColorRed          (int)0xdb172dff
#define HexColorOrange       (int)0xf78a2dff
#define HexColorYellow       (int)0xffbe3cff
#define HexColorGreenLight   (int)0xb3c123ff
#define HexColorGreenDark    (int)0x158e52ff
#define HexColorSecondary    (int)0x25255eff
#define HexColorBlueDark     (int)0x3672d4ff
#define HexColorBlueLight    (int)0x45b4d0ff
#define HexColorPurpleDark   (int)0x67378eff
#define HexColorPurpleLight  (int)0xca56d3ff
#define HexColorPink         (int)0xfc6c90ff
#define HexColorBrownDark    (int)0x561920ff
#define HexColorBrownLight   (int)0x954a44ff
#define HexColorBlueDarkT    (int)0x3672d47d

// Note: Above declared hex are only used for GuiSetStyle(...) functions

#define ColorPrimary      (Color){0x02, 0x1A, 0x40, 0xFF}
#define ColorGrayDarkest  (Color){0x47, 0x56, 0x69, 0xFF}
#define ColorGrayLighter  (Color){0x82, 0x9E, 0x9F, 0xFF}
#define ColorGrayLightest (Color){0xB1, 0xC1, 0xBB, 0xFF}
#define ColorBackground   (Color){0xFF, 0xFA, 0xDD, 0xFF}
#define ColorRed          (Color){0xDB, 0x17, 0x2D, 0xFF}
#define ColorOrange       (Color){0xF7, 0x8A, 0x2D, 0xFF}
#define ColorYellow       (Color){0xFF, 0xBE, 0x3C, 0xFF}
#define ColorGreenLight   (Color){0xB3, 0xC1, 0x23, 0xFF}
#define ColorGreenDark    (Color){0x15, 0x8E, 0x52, 0xFF}
#define ColorSecondary    (Color){0x25, 0x25, 0x5E, 0xFF}
#define ColorBlueDark     (Color){0x36, 0x72, 0xD4, 0xFF}
#define ColorBlueLight    (Color){0x45, 0xB4, 0xD0, 0xFF}
#define ColorPurpleDark   (Color){0x67, 0x37, 0x8E, 0xFF}
#define ColorPurpleLight  (Color){0xCA, 0x56, 0xD3, 0xFF}
#define ColorPink         (Color){0xFC, 0x6C, 0x90, 0xFF}
#define ColorBrownDark    (Color){0x56, 0x19, 0x20, 0xFF}
#define ColorBrownLight   (Color){0x95, 0x4A, 0x44, 0xFF}

#define ColorBlueDarkT    (Color){0x36, 0x72, 0xD4, 0x7D}
#define ColorBlueLightT   (Color){0x45, 0xB4, 0xD0, 0x7D}

#define ColorWhite        RAYWHITE
#define ColorBlack        BLACK
#define ColorTrBlack      (Color){0, 0, 0, 200}

#endif

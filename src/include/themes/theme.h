#ifndef THEMES_H
#define THEMES_H

// clang-format off
#include "../colors.h"
#include "../ext/raygui.h"
#include "ibmplexmonofont.h"
// clang-format on

#define LIGHT_PROP_COUNT 26
static const GuiStyleProp LightThemeProps[LIGHT_PROP_COUNT] = {
    {DEFAULT, TEXT_LINE_SPACING, (int)24},
    {DEFAULT, BACKGROUND_COLOR, HexColorBackground},

    {LABEL, TEXT_COLOR_NORMAL, HexColorPrimary},

    {BUTTON, TEXT_COLOR_NORMAL, HexColorPrimary},
    {BUTTON, TEXT_COLOR_FOCUSED, HexColorBackground},
    {BUTTON, TEXT_COLOR_PRESSED, HexColorPrimary},
    {BUTTON, TEXT_COLOR_DISABLED, HexColorGrayLighter},
    {BUTTON, BASE_COLOR_NORMAL, HexColorGrayLightest},
    {BUTTON, BASE_COLOR_FOCUSED, HexColorGrayLighter},
    {BUTTON, BASE_COLOR_PRESSED, HexColorGrayLightest},
    {BUTTON, BASE_COLOR_DISABLED, HexColorGrayLightest},
    {BUTTON, BORDER_COLOR_NORMAL, HexColorGrayDarkest},
    {BUTTON, BORDER_COLOR_FOCUSED, HexColorGrayDarkest},
    {BUTTON, BORDER_COLOR_PRESSED, HexColorBlueLight},
    {BUTTON, BORDER_COLOR_DISABLED, HexColorGrayDarkest},

    {BUTTON, TEXT_PADDING, 8},

    {STATUSBAR, BASE_COLOR_NORMAL, HexColorGrayLightest},
    {STATUSBAR, TEXT_COLOR_NORMAL, HexColorPrimary},

    {LISTVIEW, TEXT_COLOR_NORMAL, HexColorPrimary},
    {LISTVIEW, TEXT_COLOR_FOCUSED, HexColorSecondary},
    {LISTVIEW, TEXT_COLOR_PRESSED, HexColorPrimary},
    {LISTVIEW, BORDER_COLOR_NORMAL, HexColorSecondary},
    {LISTVIEW, BORDER_COLOR_FOCUSED, HexColorGrayDarkest},
    {LISTVIEW, BORDER_COLOR_PRESSED, HexColorPrimary},
    {LISTVIEW, BASE_COLOR_FOCUSED, HexColorGrayLightest},
    {LISTVIEW, BASE_COLOR_PRESSED, HexColorBlueDarkT},
};

void LoadAppFont() { GuiLoadStyleIBMPlexMonoFont(); }

// void LoadAppLightTheme();
void LoadAppDarkTheme();

void LoadAppLightTheme() {
    for (int i = 0; i < LIGHT_PROP_COUNT; i++) {
        GuiSetStyle(
            LightThemeProps[i].controlId, LightThemeProps[i].propertyId,
            LightThemeProps[i].propertyValue
        );
    }
}

#endif

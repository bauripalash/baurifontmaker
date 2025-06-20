#ifndef THEMES_H
#define THEMES_H

// clang-format off
#include "../colors.h"

#include "../../external/raylib.h"
#include "../../external/raygui.h"
#include "../exported/bgmlogo.h"
#include "../exported/ibmplexmonofont.h"
// clang-format on

static const GuiStyleProp LightThemeProps[] = {
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

    {TEXTBOX, BASE_COLOR_PRESSED, HexColorBackground},
    {TEXTBOX, TEXT_COLOR_PRESSED, HexColorPrimary},
    {TEXTBOX, TEXT_COLOR_NORMAL, HexColorPrimary},
    {TEXTBOX, TEXT_COLOR_FOCUSED, HexColorPrimary},
    {TEXTBOX, BORDER_COLOR_PRESSED, HexColorGrayLighter},
    {TEXTBOX, BORDER_COLOR_FOCUSED, HexColorGrayDarkest},

    {SLIDER, BORDER_COLOR_NORMAL, HexColorGrayLighter},
    {SLIDER, BORDER_COLOR_FOCUSED, HexColorGrayDarkest},
    {SLIDER, BORDER_COLOR_PRESSED, HexColorGrayDarkest},

    {SLIDER, BASE_COLOR_NORMAL, HexColorBackground},  // Slider background
    {SLIDER, BASE_COLOR_FOCUSED, HexColorBackground}, // What is this used for??
    {SLIDER, BASE_COLOR_PRESSED, HexColorGrayDarkest}, // Slider normal color

    {SLIDER, TEXT_COLOR_NORMAL, HexColorPrimary}, // What is this used for??
    {SLIDER, TEXT_COLOR_FOCUSED,
     HexColorPrimary}, // Slider color on hover/focus

    {SLIDER, TEXT_COLOR_PRESSED, HexColorPrimary}, // Slider color when pressed
    {LABEL, TEXT_COLOR_PRESSED, HexColorPrimary},
    {LABEL, TEXT_COLOR_FOCUSED, HexColorPrimary},

};

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

void LoadAppFont() { GuiLoadStyleIBMPlexMonoFont(); }

void SetAppIcon() {
    Image logo = {
        .data = BGMLOGO_DATA,
        .width = BGMLOGO_WIDTH,
        .height = BGMLOGO_HEIGHT,
        .format = BGMLOGO_FORMAT,
        .mipmaps = 1
    };

    SetWindowIcon(logo);
}

// void LoadAppLightTheme();
void LoadAppDarkTheme();

void LoadAppLightTheme() {
    for (int i = 0; i < ARRAY_COUNT(LightThemeProps); i++) {
        GuiSetStyle(
            LightThemeProps[i].controlId, LightThemeProps[i].propertyId,
            LightThemeProps[i].propertyValue
        );
    }
}

#endif

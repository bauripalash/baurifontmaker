#ifndef SAVEFONT_H
#define SAVEFONT_H

#include "gui.h"
typedef enum {
    SAVEFONT_OK,
    SAVEFONT_ERR,
} SaveFontError;

SaveFontError SaveFontToFile(const char *filename, Gui *ui);

#endif

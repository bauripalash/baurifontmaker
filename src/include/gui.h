#ifndef _GUI_H_
#define _GUI_H_

#include "mlib/m-string.h"
#include "mlib/m-dict.h"
#include "fontvalue.h"

#include <stdbool.h>
#include <stdint.h>

DICT_SET_DEF(flipset, uint8_t)
DICT_SET_DEF(itemlist, string_t, STRING_OPLIST)

typedef struct Gui {
	int winWidth;
	int winHeight;
	const char * title;
	flipset_t flipped;
	FontValue * value;
	int btnHeight;
	int btnWidth;
	bool enableGrid;
	int gridGuideWidth;
	itemlist_t items;


	
} Gui;

Gui * NewGUI();
void FreeGui(Gui * ui);
void GuiMain();

#endif

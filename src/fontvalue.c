#include "include/fontvalue.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FontValue * NewFontValue(){
	FontValue * fv = (FontValue*)calloc(1, sizeof(FontValue));
	return fv;
}

void FreeFontValue(FontValue * fv) {
	free(fv);
}

uint8_t FlipFontValue(FontValue *fv, uint8_t col, uint8_t row){
	uint8_t val = fv->bits[row];
	fv->bits[row] = val ^ (1 << col);
	return val;
}

bool GetBool(FontValue *fv, uint8_t col, uint8_t row){
	if (row >= 8 || col >= 8) {
		return false;
	}

	uint8_t bts = fv->bits[row];
	uint8_t val = (bts >> col) & 1;

	return val == 1;
}

void LogFontValue(FontValue *fv){
	printf("[ ");
	for (int i = 0; i < 8; i++) {
		printf("0x%X, ", fv->bits[i]);
	}
	printf(" ]\n");
}

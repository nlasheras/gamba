#pragma once
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "gba_types.h"

typedef struct {
    u8*  data;
    u16* palette;
    int  width;
    int  height;
} Image;

void image_load_pcx(Image* image, const u8* pcx);
void image_free(Image* image);

#endif // _IMAGE_H_


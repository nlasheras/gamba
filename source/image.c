#include "image.h"

#include <gba_video.h>
#include <pcx.h>

#include <stdlib.h>

typedef struct { unsigned char r, g, b; } pcx_rgb24;

void image_load_pcx(Image* image, const u8* pcx)
{
    pcx_rgb24* pal;

    pcx_header* hdr = (pcx_header*)pcx;
    pcx += sizeof(pcx_header); //move past the header

    if(hdr->bpp != 8)
       return;
    if (hdr->version != 5)
        return;

    image->width  = hdr->x2 - hdr->x1 + 1;
    image->height = hdr->y2 - hdr->y1 + 1;

    int size = image->width * image->height;

    int count = 0;

    image->data = (u8*)malloc(size);
    image->palette = (u16*)malloc(256*2);

    while(count < size)
    {
       u8 c = *pcx++;
       if(c < 192) {
          image->data[count++] = c;
       } else { 
          int run = c - 192;
          c = *pcx++;
          for(int i = 0; i < run; i++) 
          {
            image->data[count++] = c;
          }
       }
    }

    if (*pcx != 0x0C)
        return;

    pal = (pcx_rgb24*)(pcx + 1);
    for(int i = 0; i < 256; i++)
    {
       image->palette[i] = RGB8(pal[i].r, pal[i].g, pal[i].b);
    }
}

void image_free(Image* image)
{
    free(image->data);
    free(image->palette);
}


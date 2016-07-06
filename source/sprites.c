#include "sprites.h"

#include <gba_video.h>
#include <gba_dma.h>
#include <pcx.h>

#include <stdlib.h>

#include "test_sheet_pcx.h"

SpriteEntry OAMCopy[128];
int sm_firstFreeSprite = 0;

#define USE_DMA
void InitOAM()
{
    for(int i = 0; i < 128; i++)
        OAMCopy[i].attribute[0] = OBJ_DISABLE;
}

void UpdateOAM()
{
    const int oamSize = 128 * sizeof(SpriteEntry);
#ifdef USE_DMA
    dmaCopy(OAMCopy, OAM, oamSize);
#else
    for(int i = 0; i < size/4 ; i++)
        ((u32*)OAM)[i] = ((u32*)OAMCopy)[i];
#endif
}

// TODO: find free sprite
void InitSprite(Sprite* sprite, int gfxID)
{
    const int oamIdx = sm_firstFreeSprite++;

    sprite->gfxID = gfxID;
    
    SpriteEntry* oam = &OAMCopy[oamIdx];
    sprite->oam = oam;

    oam->attribute[0] = OBJ_256_COLOR | OBJ_SHAPE(0);  
    oam->attribute[1] = OBJ_SIZE(1);
    oam->attribute[2] = gfxID;
}

// Update the sprites OAM entry to reflect new position
void UpdateSprite(Sprite* sprite)
{
   sprite->oam->attribute[1] &= 0xFE00;
   sprite->oam->attribute[1] |= (sprite->x & 0x01FF);
 
   sprite->oam->attribute[0] &= 0xFF00;
   sprite->oam->attribute[0] |= (sprite->y & 0x00FF);
}

void CopyTiles(const u8* src, u16* dst, int width)
{
    const int stride = width;
    for(int ti=0;ti<width/16;++ti)
    {
        for(int n=0;n<4;++n)
        {
            const u8* src2 = src + ti*16 + (n/2)*8*stride + (n%2)*8;
            u16* dst2 = dst + ti*4*8*4 + n*8*4;
            for(int y=0;y<8;++y)
            {
                for(int x=0;x<4;++x){
                    dst2[x + y*4] = src2[2*x + y*stride] | (src2[2*x+1 + y*stride] << 8);
                }
            }
        }
    }
}

typedef struct { unsigned char r, g, b; } pcx_rgb24;

void LoadPCX(const u8* pcx, u16* sprites, u16* palette)
{
    pcx_rgb24* pal;

    pcx_header* hdr = (pcx_header*)pcx;
    pcx += sizeof(pcx_header); //move past the header

    int imagewidth  = hdr->x2 - hdr->x1 + 1;
    int imageheight = hdr->y2 - hdr->y1 + 1;

    int size = imagewidth * imageheight;

    if(hdr->bpp != 8)
       return;

    int count = 0;

    u8* buffer = (u8*)malloc(size);

    while(count < size)
    {
       u8 c = *pcx++;
       if(c < 192) {
          buffer[count++] = c;
       } else { 
          int run = c - 192;
          c = *pcx++;
          for(int i = 0; i < run; i++) 
          {
            buffer[count++] = c;
          }
       }
    }

    CopyTiles(buffer, sprites, imagewidth);

    free(buffer);

    if (hdr->version != 5)
        return;
    if (*pcx != 0x0C)
        return;

    pal = (pcx_rgb24*)(pcx + 1);
    for(int i = 0; i < 256; i++)
    {
       palette[i] = RGB8(pal[i].r, pal[i].g, pal[i].b);
    }
}

void LoadSpriteSheet()
{
    LoadPCX(test_sheet_pcx, SPRITE_GFX, SPRITE_PALETTE);
}


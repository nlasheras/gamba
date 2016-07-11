#include "sprites.h"
#include "image.h"

#include <gba_video.h>
#include <gba_dma.h>
#include <pcx.h>

#include <stdlib.h>

#include "test_sheet_pcx.h"

SpriteEntry OAMCopy[128];

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

int findFreeSprite()
{
    for(int i = 0; i < 128; i++)
        if (OAMCopy[i].attribute[0] == OBJ_DISABLE)
            return i;

    return -1;
}

void InitSprite(Sprite* sprite, int gfxID)
{
    const int oamIdx = findFreeSprite();

    sprite->gfxID = gfxID;
    
    SpriteEntry* oam = &OAMCopy[oamIdx];
    sprite->oam = oam;

    oam->attribute[0] = OBJ_256_COLOR | OBJ_SHAPE(0);  
    oam->attribute[1] = OBJ_SIZE(1);
    oam->attribute[2] = gfxID;
}

void FreeSprite(Sprite* sprite)
{
    sprite->oam->attribute[0] = OBJ_DISABLE;
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

void LoadSpriteSheet()
{
    Image sheet;
    Image_LoadPCX(&sheet, test_sheet_pcx);
    
    CopyTiles(sheet.data, SPRITE_GFX, sheet.width);

    for(int i = 0; i < 256; i++)
        SPRITE_PALETTE[i] = sheet.palette[i];

    Image_Free(&sheet);
}


#include "background.h" 
#include "image.h"
#include "random.h"

#include <gba_video.h>
#include <gba_dma.h>

#include "background_sheet_pcx.h"

// layer 0: parallax 256 color 256x256
// layer 1: background 256 color 256x256
// layers 2, 3: unused

u16* sm_parallaxMap = 0;
u16* sm_backgroundMap = 0;

#define USE_DMA

void _generateSpritesAndPalette()
{
#ifdef USE_DMA
    dmaCopy(SPRITE_PALETTE, BG_COLORS, 256*sizeof(u16));
#else
	u16* pal = BG_COLORS;
	for(int i=0; i<256; i++)
		*pal++ = SPRITE_PALETTE[i];
#endif

    const int tileSize = 4 * 8;
    for(int s=0;s<256;++s)
    {
        u16* dst = ((u16*)VRAM) + s*tileSize; 
        for(int x=0;x<4*8;++x)
            *(dst + x) = s << 8 | s;
    }
}

void CopyBackgroundTilesToVRAM(Image* image)
{
    const int tileSize = 4 * 8;
    int count = 0;
    for(int iy=0;iy<image->height/8;++iy)
    {
        for(int ix=0;ix<image->width/8;++ix)
        {
            u16* dst = ((u16*)VRAM) + count*tileSize; 
            u8* src = image->data + iy*image->width*8 + ix*8;

            for(int y=0;y<8;++y)
                for(int x=0;x<4;++x)
                {
                    const u8* curr = src + y*image->width + 2*x;
                    *(dst + 4*y + x) = (*curr) | (*(curr+1) << 8); 
                }

            ++count;
        }
    }
}

void LoadBackgroundSpriteSheet()
{
    Image sheet;
    Image_LoadPCX(&sheet, background_sheet_pcx);
    
    CopyBackgroundTilesToVRAM(&sheet);

    for(int i = 0; i < 256; i++)
        BG_COLORS[i] = sheet.palette[i];

    Image_Free(&sheet);
}

void InitMap()
{
    sm_parallaxMap = (u16*)MAP_BASE_ADR(31);
    BGCTRL[0] = SCREEN_BASE(31) | BG_SIZE_0 | BG_256_COLOR;
	BG_OFFSET[0].x = 0; BG_OFFSET[0].y = 0;

    sm_backgroundMap = (u16*)MAP_BASE_ADR(15);
    BGCTRL[1] = SCREEN_BASE(15) | BG_SIZE_0 | BG_256_COLOR;
	BG_OFFSET[1].x = 0; BG_OFFSET[1].y = 0;

    LoadBackgroundSpriteSheet();

    // 256x256 = 32x32 tiles  
    for(int i=0;i<32*32;++i)
	    *(sm_backgroundMap + i) = rand(1, 12);


    int count = 0;
    for(int i=0;i<32*32;++i)
	    *(sm_parallaxMap + i) = rand(1, 100) > 1 ? 0 : 12 + (count++ % 3);
}

int sm_current = 0;
int sm_scrollX = 0;
void UpdateScroll()
{
    ++sm_scrollX;
	BG_OFFSET[1].x = sm_scrollX / 3;
	BG_OFFSET[0].x = sm_scrollX;
    
    while ((sm_scrollX/3) - sm_current > 8)
    {
        const int col = sm_current / 8;
        u16* dst = sm_backgroundMap + (col%32);
        for(int i=0;i<16;++i)
	        *(dst + i*32) = rand(1, 12);

        sm_current += 8;
    }
	
}


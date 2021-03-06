#include "background.h" 
#include "random.h"

#include <gba_video.h>
#include <gba_dma.h>

#include "background_bin.h"

// layer 0: parallax 256 color 256x256
// layer 1: background 256 color 256x256
// layers 2, 3: unused

u16* sm_parallaxMap = 0;
u16* sm_backgroundMap = 0;

void internal_load_sprite_sheet()
{
    u16* pal = (u16*)background_bin;
    int pal_size = 256 * sizeof(u16);

    u8* sprites = (u8*)(background_bin + pal_size);

    dmaCopy(pal, BG_COLORS, pal_size);
    dmaCopy(sprites, (u32*)VRAM, 12 * 8 * 8 + 2 * 32 * 64) ;
}

void background_init()
{
    // 1 map_base_addr = 1<<11 bytes = 1 full BG_SIZE_0 map
    sm_parallaxMap = (u16*)MAP_BASE_ADR(31);
    BGCTRL[0] = SCREEN_BASE(31) | BG_SIZE_0 | BG_256_COLOR;
    BG_OFFSET[0].x = 0; BG_OFFSET[0].y = 0;

    sm_backgroundMap = (u16*)MAP_BASE_ADR(30);
    BGCTRL[1] = SCREEN_BASE(30) | BG_SIZE_0 | BG_256_COLOR;
    BG_OFFSET[1].x = 0; BG_OFFSET[1].y = 0;

    internal_load_sprite_sheet();

    // 256x256 = 32x32 tiles  
    for (int i = 0; i < 32 * 32; ++i)
        *(sm_backgroundMap + i) = rand(1, 11);

    // for the parallax map I have to sprites of 32*64 (4x8 blocks) 
    // starting from sprite 12
    int count = 12;
    const int deco1_posx = 1;
    const int deco2_posx = 15;
    const int bottom_tile_y = 20;
    for (int j = 0; j < 8; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            *(sm_parallaxMap +  (bottom_tile_y - 8 + j)*32 + i + deco1_posx) = count;
            ++count;
        }
    }
    for (int j = 0; j < 8; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            *(sm_parallaxMap +  (bottom_tile_y - 8 + j)*32 + i + deco2_posx) = count;
            ++count;
        }
    }

}

int sm_current = 0;
int sm_scrollX = 0;
int sm_scrollSpeedX = 0;

int background_get_scroll_speed()
{
    return sm_scrollSpeedX;
}

void background_set_scroll_speed(int speed)
{
    sm_scrollSpeedX = speed;
}

void background_update()
{
    sm_scrollX += sm_scrollSpeedX;
    BG_OFFSET[1].x = sm_scrollX / 3;
    BG_OFFSET[0].x = sm_scrollX;

    while ((sm_scrollX / 3) - sm_current > 8)
    {
        const int col = sm_current / 8;
        u16* dst = sm_backgroundMap + (col % 32);
        for (int i = 0; i < 16; ++i)
            *(dst + i * 32) = rand(1, 12);

        sm_current += 8;
    }

}


#include <gba.h> 
#include "background.h"

int game_main();

int main_test_mode3()
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    u16* screen = (u16*)VRAM;
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
            screen[x + y * SCREEN_WIDTH] = RGB8(0xFF - SCREEN_WIDTH + x, 0xFF - SCREEN_WIDTH + y, 88);

    while (1)
    {
        VBlankIntrWait();
    }
}

#include <stdio.h>

#include "graphics_bin.h"

int main_test_console()
{
    irqInit();

    consoleDemoInit();

    // ansi escape sequence to clear screen and home cursor
    iprintf("\x1b[2J");

    // ansi escape sequence to set print co-ordinatesa
    // /x1b[line;columnH
    const int number = 42;
    iprintf("\x1b[0;8HHello World %d", number);

    return 0;
}

void mem_copy32(const void* src, void* dst, int numBytes)
{
    for (int i = 0; i < numBytes / 4; ++i)
    {
        ((u32*)dst)[i] = ((u32*)src)[i];
    }
}

extern SpriteEntry sm_OAMCopy[128];
int main_test_sprites()
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_0 | OBJ_1D_MAP;
    REG_DISPCNT |= OBJ_ENABLE;

    u16* pal = (u16*)graphics_bin;
    int pal_size = 256 * sizeof(u16);

    u8* sprites = (u8*)(graphics_bin + pal_size);

    dmaCopy(pal, SPRITE_PALETTE, pal_size);
    dmaCopy(sprites, SPRITE_GFX, 12 * 16 * 16);

    for (int i = 0; i < 128; ++i)
    {
        sm_OAMCopy[i].attribute[0] = OBJ_DISABLE;
    }
    dmaCopy(sm_OAMCopy, OAM, 128*sizeof(SpriteEntry));

    SpriteEntry* sprite = &sm_OAMCopy[0];
    sprite->attribute[0] = OBJ_256_COLOR | OBJ_SHAPE(0);
    sprite->attribute[1] = OBJ_SIZE(1); 
    sprite->attribute[2] = OBJ_CHAR(0 * 8);

    int tick = 0;
    int gfxID = 0;
    int x = 10, y  = 10;
    while (true)
    {
        ++tick;

        if (tick % 5 == 0)
        {
            ++x;
            if (x > SCREEN_WIDTH) x = 0;
        }
        sprite->attribute[0] |= OBJ_Y(y);
        sprite->attribute[1] &= 0xFE00;
        sprite->attribute[1] |= OBJ_X(x);
 
        sm_OAMCopy[1].attribute[1] = x;
        sm_OAMCopy[1].attribute[2] = tick;

        VBlankIntrWait();

        u16* ptr = (u16*)VRAM;
        ptr[0] = tick;
        ptr[1] = gfxID;
        ptr[2] = x;
        ptr[3] = y;

        if (tick % 10 == 0)
            gfxID = (gfxID + 1) % 12;
    
        dmaCopy(sm_OAMCopy, OAM, 1*sizeof(SpriteEntry));

        //mem_copy32(sprite, OAM, 128*sizeof(SpriteEntry));
        dmaCopy(sprites + gfxID * 16 * 16, SPRITE_GFX, 1 * 16 * 16);
    }
}

int main()
{
    //game_main();
    //main_test_console();
    main_test_sprites();
}

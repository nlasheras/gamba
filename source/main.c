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

SpriteEntry oamCopy[128];
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
        oamCopy[i].attribute[0] = OBJ_DISABLE;
    }
    dmaCopy(oamCopy, OAM, 128*sizeof(SpriteEntry));

    SpriteEntry* sprite = &oamCopy[0];
    sprite->attribute[0] = OBJ_256_COLOR | OBJ_SHAPE(0);
    sprite->attribute[1] = OBJ_SIZE(1); 
    sprite->attribute[2] = OBJ_CHAR(0 * 8);

    int tick = 0;
    int gfxID = 0;
    int x = 10, y  = 10;
    while (true)
    {
        ++tick;

        sprite->attribute[0] |= OBJ_Y(y);
        sprite->attribute[1] |= OBJ_X(x);
 
        VBlankIntrWait();

        if (tick > 10 )
            gfxID = gfxID % 12;

        dmaCopy(oamCopy, OAM, 128*sizeof(SpriteEntry));
        dmaCopy(sprites + gfxID * 16 * 16, SPRITE_GFX, 1 * 16 * 16);
    }
}

int main()
{
    game_main();
    //main_test_console();
    //main_test_sprites();
}

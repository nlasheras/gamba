#include <gba.h> 

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
#include <stdlib.h>

int main_test_console()
{
    irqInit();

    consoleDemoInit();

    // ansi escape sequence to clear screen and home cursor
    iprintf("\x1b[2J");

    int number = 42;

    // ansi escape sequence to set print co-ordinatesa
    // /x1b[line;columnH
    iprintf("\x1b[0;8HHello World %d", number);

    return 0;
}

int main()
{
    game_main();
}

#include <gba.h> 

#include "sprites.h" 
#include "background.h"

#define IS_KEY_PRESSED(k) (!(REG_KEYINPUT & (k)))

void GetInput(Sprite* sp)
{
    if (IS_KEY_PRESSED(KEY_UP))
        sp->y--;

    if (IS_KEY_PRESSED(KEY_DOWN))
        sp->y++;

    if (IS_KEY_PRESSED(KEY_LEFT))
        sp->x--;
 
    if (IS_KEY_PRESSED(KEY_RIGHT))
        sp->x++;
 
    if (IS_KEY_PRESSED(KEY_A))
        sp->oam->attribute[1] ^= OBJ_HFLIP;

    if (IS_KEY_PRESSED(KEY_B))
        sp->oam->attribute[1] ^= OBJ_VFLIP;
}
 
int main_game(void)
{
	irqInit();
	irqEnable(IRQ_VBLANK);

    REG_DISPCNT  = MODE_0 | OBJ_1D_MAP;
    REG_DISPCNT |= OBJ_ENABLE;
    REG_DISPCNT |= BG0_ENABLE | BG1_ENABLE;

    InitOAM();
    LoadSpriteSheet();
    InitMap();

    Sprite sprite1, sprite2;

    InitSprite(&sprite1, 0);
    sprite1.x = 10; sprite1.y = 10;

    InitSprite(&sprite2, 2*4*2); // 3rd 256 colour sprites
    sprite2.x = 100; sprite2.y = 30;

	while (1) 
    {
        GetInput(&sprite1);
        UpdateSprite(&sprite1);
        UpdateSprite(&sprite2);

		VBlankIntrWait();
        
        UpdateOAM();
        UpdateScroll();
    }
}

int main_test_mode3(void)
{
	irqInit();
	irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_3 | BG2_ENABLE;   
    
    u16* screen = (u16*)VRAM;
    for(int y = 0; y < SCREEN_HEIGHT; y++)  
        for(int x = 0; x < SCREEN_WIDTH; x++)   
            screen[x + y * SCREEN_WIDTH] = RGB8(0xFF - SCREEN_WIDTH + x, 0xFF - SCREEN_WIDTH + y, 88);  

	while (1) 
    {
		VBlankIntrWait();
    }
}

int main(void)
{
    main_game();
}

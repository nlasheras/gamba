#include <gba.h> 

#include "sprites.h" 
#include "background.h"
#include "random.h"
#include "enemies.h"

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

    random_init(31173);

    REG_DISPCNT  = MODE_0 | OBJ_1D_MAP;
    REG_DISPCNT |= OBJ_ENABLE;
    REG_DISPCNT |= BG0_ENABLE | BG1_ENABLE;

    InitOAM();
    LoadSpriteSheet();
    InitMap();

    Sprite player;

    InitSprite(&player, 0);
    player.x = 10; player.y = 10;
   
    InitEnemies();

	while (1) 
    {
        GetInput(&player);
        UpdateSprite(&player);
        
        UpdateEnemies();

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

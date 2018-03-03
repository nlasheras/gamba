#include <gba.h> 

#include "sprites.h" 
#include "background.h"
#include "random.h"
#include "enemies.h"
#include "bullets.h"
#include "explosions.h"

#define IS_KEY_PRESSED(k) (!(REG_KEYINPUT & (k)))

int sm_ticksUntilShoot = 0;
const int DELAY_SHOOT_TICKS = 10;
void TryToShoot(Sprite* player)
{
	if (sm_ticksUntilShoot <= 0)
	{
		InitBullet(player->x, player->y);
		sm_ticksUntilShoot = DELAY_SHOOT_TICKS;
	}
}

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
	{
		TryToShoot(sp);
	}

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
   
    enemies_init_all();

	while (1) 
    {
        GetInput(&player);
        UpdateSprite(&player);
        
		UpdateBullets();
        enemies_update_all();
		UpdateExplosions();
		entities_update_all();

		VBlankIntrWait();
        
        UpdateOAM();
        UpdateScroll();

		if (sm_ticksUntilShoot > 0)
			--sm_ticksUntilShoot;
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

#include <gba.h> 

#include "sprites.h" 
#include "background.h"
#include "random.h"

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

typedef struct {
    Sprite sprite;
    int hp;
    bool enabled;
} Enemy;
 
#define MAX_ENEMIES 16
Enemy sm_enemies[MAX_ENEMIES];

int findEmptyEnemy()
{
    for(int i=0;i<MAX_ENEMIES;++i)
        if (!sm_enemies[i].enabled)
            return i;
    return -1;
}

int sm_enemyCount = 0;

void initEnemy(int x, int y)
{
    int idx = findEmptyEnemy();
    if (idx == -1)
        return;

    sm_enemies[idx].enabled = true;

    // enemy is the 3rd 256 colour sprite
    InitSprite(&(sm_enemies[idx].sprite), 2*4*2); 
    sm_enemies[idx].sprite.x = x; 
    sm_enemies[idx].sprite.y = y;
    
    UpdateSprite(&(sm_enemies[idx].sprite));

    ++sm_enemyCount;
}

void freeEnemy(Enemy* enemy)
{
    enemy->enabled = false;
    
    FreeSprite(&enemy->sprite);
    
    --sm_enemyCount;
}

void initEnemies()
{
    int count = rand(1, 6);
    for (int i=0;i<count;++i)
    {
        int x = 100 + 20*i;
        int y = rand(10, 150-16);
        initEnemy(x, y);
    }

}

void updateEnemies()
{
    for(int i=0;i<MAX_ENEMIES;++i)
    {
        Enemy* enemy = &(sm_enemies[i]);
        if (!enemy->enabled)
            continue;
        
        enemy->sprite.x -= 1;

        UpdateSprite(&(enemy->sprite));

        if (enemy->sprite.x < -16)
            freeEnemy(enemy);
    }

    if (sm_enemyCount < 8)
    {
        if (rand(0, 100) < 5)
            initEnemy(240, rand(10, 150-16));
    }
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
   
    initEnemies();

	while (1) 
    {
        GetInput(&player);
        UpdateSprite(&player);
        
        updateEnemies();

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

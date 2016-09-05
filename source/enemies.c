#include <gba.h> 

#include "enemies.h"
#include "sprites.h" 
#include "random.h" 

#define MAX_ENEMIES 16
Enemy sm_enemies[MAX_ENEMIES];
int sm_enemyCount = 0;

int findEmptyEnemy()
{
    for(int i=0;i<MAX_ENEMIES;++i)
        if (!sm_enemies[i].enabled)
            return i;
    return -1;
}

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

void InitEnemies()
{
    int count = rand(1, 6);
    for (int i=0;i<count;++i)
    {
        int x = 100 + 20*i;
        int y = rand(10, 150-16);
        initEnemy(x, y);
    }
}

void UpdateEnemies()
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
        if (rand(0, 100) < 1)
            initEnemy(240, rand(10, 150-16));
    }
}


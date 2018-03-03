#include <gba.h> 

#include "enemies.h"
#include "sprites.h" 
#include "random.h" 
#include "bullets.h"
#include "collision.h"

#define MAX_ENEMIES 16
Enemy sm_enemies[MAX_ENEMIES];
int sm_enemyCount = 0;

const int enemyFrames[2] = { 4, 5 };
const int numFrames = sizeof(enemyFrames) / sizeof(int);
const int ENEMY_ANIMATION_TIME = 15;

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

	Enemy* enemy = &(sm_enemies[idx]);
	enemy->enabled = true;

    InitSprite(&(enemy->sprite), 4*SPRITE_OFFSET); 
    enemy->sprite.x = x; 
    enemy->sprite.y = y;
	enemy->animationTime = 0;
	enemy->frame = 0;
    UpdateSprite(&(enemy->sprite));

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
		UpdateEnemyAnimation(enemy);
        UpdateSprite(&(enemy->sprite));

        if (enemy->sprite.x < -16)
            freeEnemy(enemy);

		if (TestCollision(enemy))
		{
			freeEnemy(enemy);
		}
    }

    if (sm_enemyCount < 8)
    {
        if (rand(0, 100) < 1)
            initEnemy(240, rand(10, 150-16));
    }
}

void UpdateEnemyAnimation(Enemy* enemy)
{
	enemy->animationTime++;
	if (enemy->animationTime >= ENEMY_ANIMATION_TIME)
	{
		const int newFrame = (enemy->frame + 1) % numFrames;
		Sprite* sprite = &(enemy->sprite);
		FreeSprite(sprite);
		InitSprite(sprite, enemyFrames[newFrame] * SPRITE_OFFSET);
		enemy->animationTime = 0;
		enemy->frame = newFrame;
	}
}
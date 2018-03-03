#include <gba.h> 

#include "enemies.h"
#include "sprites.h" 
#include "random.h" 
#include "bullets.h"
#include "collision.h"
#include "explosions.h"
#include "entities.h"

#define MAX_ENEMIES 16
Enemy sm_enemies[MAX_ENEMIES];
int sm_enemyCount = 0;

const int sm_enemyFrameIdx[2] = { 4, 5 };
const int sm_numEnemyFrames = sizeof(sm_enemyFrameIdx) / sizeof(int);
const int ENEMY_ANIMATION_TIME = 15;

int enemy_pool_find_first_empty()
{
    for(int i=0;i<MAX_ENEMIES;++i)
        if (sm_enemies[i].entity == NULL)
            return i;
    return -1;
}

Enemy* enemy_create(int x, int y)
{
    const int idx = enemy_pool_find_first_empty();
    if (idx == -1)
        return NULL;

	Entity* entity = entity_create(ENTITY_ENEMY);
	if (entity == NULL)
		return NULL;

	Enemy* enemy = &(sm_enemies[idx]);
	enemy->entity = entity;

	entity_set_sprite(enemy->entity, 4);
    enemy->entity->x = x; 
    enemy->entity->y = y;
	enemy->animationTime = 0;
	enemy->frame = 0;

    ++sm_enemyCount;

	return enemy;
}

void enemy_free(Enemy* enemy)
{
    entity_free(enemy->entity);
    enemy->entity = NULL;
    
    --sm_enemyCount;
}

void enemies_init_all()
{
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		sm_enemies[i].entity = NULL;
	}

    int count = rand(1, 6);
    for (int i = 0; i < count; ++i)
    {
		int x = 100 + 20 * i;
		int y = rand(10, 150 - 16);
        enemy_create(x, y);
    }
}

void enemies_update_all()
{
    for(int i=0;i<MAX_ENEMIES;++i)
    {
        Enemy* enemy = &(sm_enemies[i]);
        if (enemy->entity == NULL)
            continue;
        
        enemy->entity->x -= 1;

		if (enemy->entity->x < -16)
		{
			enemy_free(enemy);
		}
		
    	enemy_update_animation_internal(enemy);

		Bullet* b = TestCollision(enemy);
		if (b != NULL)
		{
			InitExplosion(enemy->entity->x, enemy->entity->y);
			enemy_free(enemy);
			bullet_free(b);
		}
    }

    if (sm_enemyCount < 8)
    {
        if (rand(0, 100) < 1)
            enemy_create(240, rand(10, 150-16));
    }
}

void enemy_update_animation_internal(Enemy* enemy)
{
	enemy->animationTime++;
	if (enemy->animationTime >= ENEMY_ANIMATION_TIME)
	{
		const int newFrame = (enemy->frame + 1) % sm_numEnemyFrames;
		entity_set_sprite(enemy->entity, sm_enemyFrameIdx[newFrame]);
		enemy->animationTime = 0;
		enemy->frame = newFrame;
	}
}
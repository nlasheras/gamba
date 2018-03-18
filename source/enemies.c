#include "enemies.h"
#include "enemies_internal.h"
#include "random.h" 
#include "collision.h"
#include "explosions.h"
#include "entities.h"

#include <gba.h> 

#define MAX_ENEMIES 16
Enemy sm_enemies[MAX_ENEMIES];
int sm_enemyCount = 0;

const int sm_enemyFrameIdx[2] = { 4, 5 };
const int sm_numEnemyFrames = sizeof(sm_enemyFrameIdx) / sizeof(int);
const int ENEMY_ANIMATION_TIME = 15;

int enemies_pool_find_first_empty()
{
    for (int i = 0; i < MAX_ENEMIES; ++i)
        if (sm_enemies[i].entity == NULL)
            return i;
    return -1;
}

Enemy* enemy_create(int x, int y)
{
    const int idx = enemies_pool_find_first_empty();
    if (idx == -1)
        return NULL;

    Enemy* enemy = &(sm_enemies[idx]);
    Entity* entity = entity_create(ENTITY_ENEMY, enemy);
    if (entity == NULL)
        return NULL;

    enemy->entity = entity;

    entity_set_sprite(enemy->entity, 4);
    enemy->entity->x = x;
    enemy->entity->y = y;
    enemy->animationTime = 0;
    enemy->frame = 0;

    collision_init(&entity->collider, 0, 0, 16, 16);

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

Bullet* enemy_test_collision_bullets(Enemy* e)
{
    const int numBullets = entities_get_count(ENTITY_BULLET);
    for (int i = 0; i < numBullets; ++i)
    {
        Entity* entity_bullet = entities_get(ENTITY_BULLET, i);
        if (entity_bullet && collision_test(entity_bullet, e->entity))
        {
            return (Bullet*)(entity_bullet->child);
        }
    }
    return NULL;
}

void enemies_update_all()
{
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        Enemy* enemy = &(sm_enemies[i]);
        if (enemy->entity == NULL)
            continue;

        enemy->entity->x -= 1;

        if (enemy->entity->x < -16)
        {
            enemy_free(enemy);
        }

        enemy_update_animation(enemy);

        Bullet* b = enemy_test_collision_bullets(enemy);
        if (b != NULL)
        {
            explosion_create(enemy->entity->x, enemy->entity->y);
            enemy_free(enemy);
            bullet_free(b);
        }
    }

    if (sm_enemyCount < 8)
    {
        if (rand(0, 100) < 1)
            enemy_create(240, rand(10, 150 - 16));
    }
}

void enemy_update_animation(Enemy* enemy)
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
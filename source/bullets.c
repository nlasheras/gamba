#include <gba.h> 

#include "bullets.h"

#define MAX_BULLETS 16
Bullet sm_bullets[MAX_BULLETS];

int bullet_pool_find_first_empty()
{
	for (int i = 0; i < MAX_BULLETS; ++i)
		if (sm_bullets[i].entity == NULL)
			return i;
	return -1;
}

Bullet* bullet_create(int x, int y)
{
	const int idx = bullet_pool_find_first_empty();
	if (idx == -1)
		return NULL;

	Entity* entity = entity_create(2);
	if (entity == NULL)
		return NULL;

	Bullet* bullet = &(sm_bullets[idx]);
	bullet->entity = entity;
	
	entity->x = x;
	entity->y = y;

	entity_set_sprite(entity, 1);

	return bullet;
}

void bullet_free(Bullet* bullet)
{
	entity_free(bullet->entity);
	bullet->entity = NULL;
}

void bullets_update_all()
{
	for (int i = 0; i < MAX_BULLETS; ++i)
	{
		Bullet* bullet = &(sm_bullets[i]);
		if (bullet->entity)
		{
			bullet->entity->x += 2;

			if (bullet->entity->x > SCREEN_WIDTH)
				bullet_free(bullet);
		}
	}
}

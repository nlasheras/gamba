#include "explosions.h"

#define MAX_EXPLOSIONS 16
Explosion sm_explosions[MAX_EXPLOSIONS];

const int sm_explosionFrameIdx[4] = { 8, 9, 10, 11 };
const int sm_numExplosionFrames = sizeof(sm_explosionFrameIdx) / sizeof(int);
const int EXPLOSION_ANIMATION_TIME = 7;

int explosions_pool_find_first_empty()
{
	for (int i = 0; i<MAX_EXPLOSIONS; ++i)
		if (sm_explosions[i].entity == NULL)
			return i;
	return -1;
}

Explosion* explosion_create(int x, int y)
{
	const int idx = explosions_pool_find_first_empty();
	if (idx == -1)
		return NULL;

	Entity* entity = entity_create(ENTITY_EXPLOSION);
	if (entity == NULL)
		return NULL;

	Explosion* fx = &(sm_explosions[idx]);
	fx->entity = entity;

	entity_set_sprite(entity, 8);
	entity->x = x;
	entity->y = y;
	fx->animationTime = 0;
	fx->frame = 0;

	return fx;
}

void explosion_free(Explosion* fx)
{
	entity_free(fx->entity);
	fx->entity = NULL;
}

void explosions_update_all()
{
	for (int i = 0; i < MAX_EXPLOSIONS; ++i)
	{
		Explosion* fx = &(sm_explosions[i]);
		if (fx->entity)
		{
			fx->entity->x -= 1;

			fx->animationTime++;

			if (fx->animationTime >= EXPLOSION_ANIMATION_TIME)
			{
				const int newFrame = fx->frame + 1;
				if (newFrame >= sm_numExplosionFrames)
				{
					explosion_free(fx);
					continue;
				}
				else
				{
					entity_set_sprite(fx->entity, sm_explosionFrameIdx[newFrame]);
					fx->animationTime = 0;
					fx->frame = newFrame;
				}
			}
		}
	}
}

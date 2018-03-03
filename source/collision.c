#include <gba.h> 

#include "collision.h"
#include "bullets.h"

bool collision(const Bullet* b, const Enemy* e)
{
	const int bx = b->sprite.x + 8;
	const int by = b->sprite.y + 8;

	const int ex = e->entity->x;
	const int ey = e->entity->y;
	const int ew = 16;
	const int eh = 16;

	return bx > ex && bx < ex + ew && by > ey && by < ey + eh;
}

extern Bullet sm_bullets[16];
Bullet* TestCollision(const Enemy* e)
{
	for (int i = 0; i < 16; ++i)
	{
		Bullet* b = &(sm_bullets[i]);
		if(b->enabled)
		{
			if (collision(b, e))
				return b;
		}
	}
	return NULL;
}
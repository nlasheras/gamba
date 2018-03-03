#include <gba.h> 

#include "collision.h"
#include "bullets.h"

bool collision(const Bullet* b, const Enemy* e)
{
	const int bx = b->sprite.x + 8;
	const int by = b->sprite.y + 8;

	const int ex = e->sprite.x;
	const int ey = e->sprite.y;
	const int ew = 16;
	const int eh = 16;

	return bx > ex && bx < ex + ew && by > ey && by < ey + eh;
}

extern Bullet sm_bullets[16];
bool TestCollision(const Enemy* e)
{
	for (int i = 0; i < 16; ++i)
	{
		if(sm_bullets[i].enabled)
		{
			if (collision(&(sm_bullets[i]), e))
				return true;
		}
	}
	return false;
}
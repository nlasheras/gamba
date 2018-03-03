#include <gba.h> 

#include "bullets.h"
#include "sprites.h" 
#include "random.h" 


int findEmptyBullet()
{
		if (!sm_bullets[i].enabled)
			return i;
	return -1;
}

void InitBullet(int x, int y)
{
	int idx = findEmptyBullet();
	if (idx == -1)
		return;

	Bullet* bullet = &(sm_bullets[idx]);
	bullet->enabled = true;

	InitSprite(&(bullet->sprite), 1 * SPRITE_OFFSET);
	bullet->sprite.x = x;
	bullet->sprite.y = y;
	UpdateSprite(&(bullet->sprite));
}

void freeBullet(Bullet* enemy)
{
	enemy->enabled = false;

	FreeSprite(&enemy->sprite);
}

void UpdateBullets()
{
	{
		Bullet* bullet = &(sm_bullets[i]);
		if (!bullet->enabled)
			continue;

		bullet->sprite.x += 2;
		UpdateSprite(&(bullet->sprite));

		if (bullet->sprite.x > SCREEN_WIDTH)
			freeBullet(bullet);
	}
}

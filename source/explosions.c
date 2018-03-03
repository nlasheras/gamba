#include <gba.h> 

#include "explosions.h"

#define MAX_EXPLOSIONS 16
Explosion sm_explosions[MAX_EXPLOSIONS];

const int explosionFrames[4] = { 8, 9, 10, 11 };
const int numExplosionFrames = sizeof(explosionFrames) / sizeof(int);
const int EXPLOSION_ANIMATION_TIME = 7;

int findEmptyExplosion()
{
	for (int i = 0; i<MAX_EXPLOSIONS; ++i)
		if (!sm_explosions[i].enabled)
			return i;
	return -1;
}

void InitExplosion(int x, int y)
{
	int idx = findEmptyExplosion();
	if (idx == -1)
		return;

	Explosion* fx = &(sm_explosions[idx]);
	fx->enabled = true;

	InitSprite(&(fx->sprite), 8 * SPRITE_OFFSET);
	fx->sprite.x = x;
	fx->sprite.y = y;
	fx->animationTime = 0;
	fx->frame = 0;
	UpdateSprite(&(fx->sprite));
}

void freeExplosion(Explosion* fx)
{
	fx->enabled = false;

	FreeSprite(&fx->sprite);
}

void UpdateExplosions()
{
	for (int i = 0; i<MAX_EXPLOSIONS; ++i)
	{
		Explosion* fx = &(sm_explosions[i]);
		if (!fx->enabled)
			continue;
		fx->sprite.x -= 1;

		fx->animationTime++;
		if (fx->animationTime >= EXPLOSION_ANIMATION_TIME)
		{
			const int newFrame = fx->frame + 1;
			if (newFrame >= numExplosionFrames)
			{
				freeExplosion(fx);
				continue;
			}
			else
			{
				Sprite* sprite = &(fx->sprite);
				FreeSprite(sprite);
				InitSprite(sprite, explosionFrames[newFrame] * SPRITE_OFFSET);
				fx->animationTime = 0;
				fx->frame = newFrame;
			}
		}

		UpdateSprite(&(fx->sprite));
	}
}

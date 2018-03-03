#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "sprites.h" 

typedef struct {
	Sprite sprite;
	bool enabled;
	int frame;
	int animationTime;
} Explosion;

void InitExplosion(int x, int y);
void UpdateExplosions();

#endif // _EXPLOSION_H_

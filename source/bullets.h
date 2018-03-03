#pragma once
#ifndef _BULLETS_H_
#define _BULLETS_H_

#include "sprites.h" 

typedef struct {
	Sprite sprite;
	bool enabled;
} Bullet;

void InitBullet(int x, int y);
void FreeBullet(Bullet* b);
void UpdateBullets();

#endif // _BULLET_H_

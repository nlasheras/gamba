#pragma once
#ifndef _BULLETS_H_
#define _BULLETS_H_

#include "entities.h"

typedef struct {
	Entity* entity;
} Bullet;

Bullet* bullet_create(int x, int y);
void bullet_free(Bullet* b);

void bullets_update_all();

#endif // _BULLET_H_

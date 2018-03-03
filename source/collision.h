#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <gba_types.h>

typedef struct _entity Entity; // forward-declaration

typedef struct
{
	bool enabled;
	int x, y;
	int w, h;
} Collider;


void collision_init(Collider* c, int x, int y, int w, int h);
bool collision_test(const Entity* e1, const Entity* e2);

#endif // _COLLISION_H_

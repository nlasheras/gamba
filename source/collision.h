#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

typedef struct _entity Entity; // forward-declaration
typedef struct _enemy Enemy; 
typedef struct _bullet Bullet;

typedef struct
{
	bool enabled;
	int x, y;
	int w, h;
} Collider;


void collision_init(Collider* c, int x, int y, int w, int h);
bool collision_test(const Entity* e1, const Entity* e2);

Bullet* collisions_test_all_bullets(Enemy* enemy);

#endif // _COLLISION_H_

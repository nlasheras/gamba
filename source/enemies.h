#pragma once
#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#include "entities.h"

typedef struct _enemy {
    Entity* entity;
    int hp;
	int frame;
	int animationTime;
} Enemy;
 
void enemies_init_all();
void enemies_update_all();

Enemy* enemy_create(int x, int y);
void enemy_free(Enemy* e);

#endif // _ENEMIES_H_

#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "entities.h"

typedef struct {
    Entity* entity;
    int frame;
    int animationTime;
} Explosion;

Explosion* explosion_create(int x, int y);

void explosions_update_all();

#endif // _EXPLOSION_H_

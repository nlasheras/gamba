#pragma once
#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#include "sprites.h" 

typedef struct {
    Sprite sprite;
    int hp;
    bool enabled;
	int frame;
	int animationTime;
} Enemy;
 
void InitEnemies();
void UpdateEnemies();
void UpdateEnemyAnimation(Enemy* enemy);

#endif // _ENEMIES_H_

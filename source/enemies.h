#pragma once
#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#include "sprites.h" 

typedef struct {
    Sprite sprite;
    int hp;
    bool enabled;
} Enemy;
 
void InitEnemies();
void UpdateEnemies();

#endif // _ENEMIES_H_

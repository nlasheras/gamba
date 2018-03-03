#pragma once
#ifndef _ENEMIES_INTERNAL_H_
#define _ENEMIES_INTERNAL_H_

#include "enemies.h"
#include "bullets.h"

void enemy_update_animation(Enemy* e);
Bullet* enemy_test_collision_bullets(Enemy* e);

#endif // _ENEMIES_INTERNAL_H_

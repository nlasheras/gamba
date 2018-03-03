#pragma once
#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "sprites.h" 

#define ENTITY_ENEMY 1

typedef struct {
    Sprite sprite;
    bool enabled;
	int x, y;
	int type;
} Entity;
 
Entity* entity_create(int type);
void entity_free(Entity* e);
void entity_set_sprite(Entity* e, int idx);



#endif // _ENTITIES_H_

#pragma once
#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "sprites.h" 

typedef enum
{
	ENTITY_ENEMY,
	ENTITY_BULLET,
	ENTITY_EXPLOSION
} EntityType;

typedef struct {
    Sprite sprite;
    bool enabled;
	int x, y;
	EntityType type;
} Entity;
 
Entity* entity_create(EntityType type);
void entity_free(Entity* e);
void entity_set_sprite(Entity* e, int idx);

void entities_update_all();


#endif // _ENTITIES_H_

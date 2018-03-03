#include "entities.h"
#include "sprites.h" 

#define MAX_ENTITIES 32
Entity sm_entities[MAX_ENTITIES];

int entity_find_first_free_index()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (!sm_entities[i].enabled)
			return i;
	}
	return -1;
}

Entity* entity_create(int type)
{
    const int idx = entity_find_first_free_index();
    if (idx == -1)
        return NULL;

	Entity* e = &(sm_entities[idx]);
	e->enabled = true;
	e->type = type;

	InitSprite(&(e->sprite), DEFAULT_GFX_ID);

	return e;
}

void entity_set_sprite(Entity* e, int idx)
{
	SetSprite(&(e->sprite), idx * SPRITE_OFFSET);
}

void entity_free(Entity* e)
{
    e->enabled = false;
    
    FreeSprite(&e->sprite);
}

void entities_update_all()
{
    for(int i=0;i<MAX_ENTITIES;++i)
    {
        Entity* e = &(sm_entities[i]);
        if (e->enabled)
        {
			e->sprite.x = e->x;
			e->sprite.y = e->y;
			UpdateSprite(&(e->sprite));
        }
    }
}

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

Entity* entity_create(EntityType type, void* child)
{
    const int idx = entity_find_first_free_index();
    if (idx == -1)
        return NULL;

    Entity* e = &(sm_entities[idx]);
    e->enabled = true;
    e->type = type;
    e->child = child;

    sprite_init(&(e->sprite), DEFAULT_GFX_ID);

    return e;
}

void entity_set_sprite(Entity* e, int idx)
{
    sprite_set_gfx(&(e->sprite), idx * SPRITE_OFFSET);
}

void entity_free(Entity* e)
{
    e->enabled = false;
    e->child = NULL;

    sprite_free(&e->sprite);
}

void entities_update_all()
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        Entity* e = &(sm_entities[i]);
        if (e->enabled)
        {
            e->sprite.x = e->x;
            e->sprite.y = e->y;
            sprite_update(&(e->sprite));

            e->collider.x = e->x;
            e->collider.y = e->y;
        }
    }
}

int entities_get_count(EntityType type)
{
    int count = 0;
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        Entity* e = &(sm_entities[i]);
        if (e->type == type && e->enabled)
            ++count;
    }
    return count;
}

Entity* entities_get(EntityType type, int index)
{
    int count = 0;
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        Entity* e = &(sm_entities[i]);
        if (e->enabled && e->type == type)
        {
            if (count == index)
                return e;
            ++count;
        }
    }
    return NULL;
}

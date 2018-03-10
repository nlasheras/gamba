#pragma once
#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <gba_sprites.h>

#define DEFAULT_GFX_ID 3
#define SPRITE_OFFSET 8 // a 16x16 256 colour sprite uses 8 tiles

typedef struct
{
    int x, y;
    SpriteEntry* oam;
    int gfxID;
} Sprite;

void sprites_init_OAM();
void sprites_load_sprite_sheet();
void sprites_update_OAM();

void sprite_init(Sprite* sprite, int gfxID);
void sprite_update(Sprite* sprite);
void sprite_free(Sprite* sprite);
void sprite_set_gfx(Sprite* sprite, int gfxID);

#endif // _SPRITES_H_

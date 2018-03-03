#pragma once
#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <gba_sprites.h>

#define SPRITE_OFFSET 8 // a 16x16 256 colour sprite uses 8 tiles

typedef struct
{
   int x,y;
   SpriteEntry* oam;
   int gfxID;
} Sprite;

void InitOAM();
void LoadSpriteSheet();
void UpdateOAM();

void InitSprite(Sprite* sprite, int gfxID);
void UpdateSprite(Sprite* sprite);
void FreeSprite(Sprite* sprite);

#endif // _SPRITES_H_

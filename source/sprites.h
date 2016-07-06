#pragma once
#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <gba_sprites.h>

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

#endif // _SPRITES_H_


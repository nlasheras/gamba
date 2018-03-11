#include "sprites.h"

#include <gba_video.h>
#include <gba_dma.h>

#include "sprites_bin.h"

SpriteEntry sm_OAMCopy[128];

#define USE_DMA

void sprites_init_OAM()
{
    for (int i = 0; i < 128; i++)
        sm_OAMCopy[i].attribute[0] = OBJ_DISABLE;
}

void sprites_update_OAM()
{
    const int oamSize = 128 * sizeof(SpriteEntry);
#ifdef USE_DMA
    dmaCopy(sm_OAMCopy, OAM, oamSize);
#else
    for (int i = 0; i < oamSize / 4; i++)
        ((u32*)OAM)[i] = ((u32*)sm_OAMCopy)[i];
#endif
}

int oam_pool_find_fist_empty()
{
    for (int i = 0; i < 128; i++)
        if (sm_OAMCopy[i].attribute[0] == OBJ_DISABLE)
            return i;

    return -1;
}

void sprite_init(Sprite* sprite, int gfxID)
{
    const int oamIdx = oam_pool_find_fist_empty();

    sprite->gfxID = gfxID;

    SpriteEntry* oam = &sm_OAMCopy[oamIdx];
    sprite->oam = oam;

    oam->attribute[0] = OBJ_256_COLOR | OBJ_SHAPE(0);
    oam->attribute[1] = OBJ_SIZE(1);
    oam->attribute[2] = gfxID;
}

void sprite_set_gfx(Sprite* sprite, int gfxID)
{
    sprite->oam->attribute[2] = gfxID;
}

void sprite_free(Sprite* sprite)
{
    sprite->oam->attribute[0] = OBJ_DISABLE;
    sprite->oam = NULL;
}

void sprite_update(Sprite* sprite)
{
    sprite->oam->attribute[1] &= 0xFE00;
    sprite->oam->attribute[1] |= (sprite->x & 0x01FF);

    sprite->oam->attribute[0] &= 0xFF00;
    sprite->oam->attribute[0] |= (sprite->y & 0x00FF);
}


void sprites_load_sprite_sheet()
{
    u16* pal = (u16*)sprites_bin;
    int pal_size = 256 * sizeof(u16);

    u8* sprites = (u8*)(sprites_bin + pal_size);

    dmaCopy(pal, SPRITE_PALETTE, pal_size);
    dmaCopy(sprites, SPRITE_GFX, 12 * 16 * 16);
}


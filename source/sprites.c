#include "sprites.h"
#include "image.h"

#include <gba_video.h>
#include <gba_dma.h>

#include "test_sheet_pcx.h"

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

void sprites_internal_copy_tiles(const u8* src, u16* dst, int width, int height)
{
    const int stride = width;
    for (int tj = 0; tj < height / 16; ++tj)
    {
        const u8* srcLine = src + tj * width * 16;
        u16* dstLine = dst + tj * width * 8;
        for (int ti = 0; ti < width / 16; ++ti)
        {
            for (int n = 0; n < 4; ++n)
            {
                const u8* src2 = srcLine + ti * 16 + (n / 2) * 8 * stride + (n % 2) * 8;
                u16* dst2 = dstLine + ti * 4 * 8 * 4 + n * 8 * 4;
                for (int y = 0; y < 8; ++y)
                {
                    for (int x = 0; x < 4; ++x) {
                        dst2[x + y * 4] = src2[2 * x + y * stride] | (src2[2 * x + 1 + y * stride] << 8);
                    }
                }
            }
        }
    }
}

void sprites_load_sprite_sheet()
{
    Image sheet;
    image_load_pcx(&sheet, test_sheet_pcx);

    sprites_internal_copy_tiles(sheet.data, SPRITE_GFX, sheet.width, sheet.height);

    for (int i = 0; i < 256; i++)
        SPRITE_PALETTE[i] = sheet.palette[i];

    image_free(&sheet);
}


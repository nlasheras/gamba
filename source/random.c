#include "random.h"

#include <gba_types.h>

// This random is based on the Drunken Coders r256 random 
//  http://www.drunkencoders.com/tutorials/GBA/day_3.html

u16 sm_r256table[256];
int sm_r256index;

void random_init(int seed)
{
    int i, j, msb;
    j = seed;
    for (i = 0; i < 256; i++)
    {
        sm_r256table[i] = (u16)(j = j * 65539);
    }

    msb = 0x8000;

    j = 0;
    for (i = 0; i < 16; i++)
    {
        j = i * 5 + 3;
        sm_r256table[j] |= msb;
        sm_r256table[j + 1] &= ~msb;
        msb >>= 1;
    }
}

u16 r256(void)
{
    u16 r;
    r = sm_r256table[(sm_r256index + 103) & 255] ^ sm_r256table[sm_r256index & 255];
    sm_r256table[sm_r256index++ & 255] = r;
    return r;
}

int rand(int min, int max)
{
    int r = r256();
    return min + (r % (max - min));
}


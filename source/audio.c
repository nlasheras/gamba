#include "audio.h"
#include <gba_interrupt.h>

#include <maxmod.h>

#include "soundbank.h"
#include "soundbank_bin.h"

void audio_init()
{
	irqSet(IRQ_VBLANK, mmVBlank);

	mmInitDefault((mm_addr)soundbank_bin, 8);
}

void audio_update()
{
	mmFrame();
}

void audio_play_music()
{
	//mmStart(MOD_MUSIC, MM_PLAY_LOOP);
}

void audio_play_explosion()
{
	mm_sound_effect fx = {
		{ SFX_EXPLOSION } ,			// id
		(int)(1.0f * (1 << 10)),	// rate
		1,		// handle
		255,	// volume
		0,		// panning
	};
	mmEffectEx(&fx);
}

void audio_play_laser()
{
	mm_sound_effect fx = {
		{ SFX_PLAYER_SHOOT } ,		// id
		(int)(1.0f * (1 << 10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};
	mmEffectEx(&fx);
}

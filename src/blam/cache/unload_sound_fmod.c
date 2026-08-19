/* unload_sound_fmod @ 0x836BF1C8 — release a permutation's FMOD-side resources. Empty in this build
 * (the PC sound cache manages residency directly; the FMOD unload hook is a no-op stub). */

#include "headers/sound_permutation.h"

void unload_sound_fmod(sound_permutation *sound)
{
    (void)sound;
}

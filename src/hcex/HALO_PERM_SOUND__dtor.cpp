#include "../headers/hcex/HALO_PERM_SOUND.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// HALO_PERM_SOUND::~HALO_PERM_SOUND() @ 0x836B4FC0
// Trivial teardown: only asserts the crossfade DSPs were already released before destruction.
HALO_PERM_SOUND::~HALO_PERM_SOUND()
{
    if (!IGNORE_STRONG_ASSERT && (this->prevFadeOut || this->fadeIn))
        STRONG_ASSERT_DUMMY::Crash(nullptr, "prevFadeOut == 0 && fadeIn == 0",
            "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h", 260, empty_string);
}

#include "../headers/hcex/HALO_CHANNEL.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// HALO_CHANNEL::~HALO_CHANNEL() @ 0x836B5A80
// Trivial teardown: assert both queued and current slots already released their crossfade DSPs.
// The single-read of IGNORE_STRONG_ASSERT into a local (v1) is the compiler caching the global;
// short-circuiting `!IGNORE_STRONG_ASSERT && ...` reproduces the same behavior.
HALO_CHANNEL::~HALO_CHANNEL()
{
    if (!IGNORE_STRONG_ASSERT)
    {
        if (this->queued.prevFadeOut || this->queued.fadeIn)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("prevFadeOut == 0 && fadeIn == 0",
                "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h", 260, empty_string);

        if (!IGNORE_STRONG_ASSERT && (this->current.prevFadeOut || this->current.fadeIn))
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("prevFadeOut == 0 && fadeIn == 0",
                "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h", 260, empty_string);
    }
}

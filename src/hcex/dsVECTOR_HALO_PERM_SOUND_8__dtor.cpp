#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/HALO_PERM_SOUND.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

// dsVECTOR<HALO_PERM_SOUND,8>::~dsVECTOR() @ 0x836B6BE8
// Destroy the perm-sound table. Per element the compiler inlined ~HALO_PERM_SOUND (which is just
// the crossfade-DSP-released assert), then frees the backing store. Reversed per-instantiation.
template<>
dsVECTOR<HALO_PERM_SOUND, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        HALO_PERM_SOUND *pElem = &this->pData[i];
        if (!IGNORE_STRONG_ASSERT && (pElem->prevFadeOut || pElem->fadeIn)) // inlined ~HALO_PERM_SOUND
            STRONG_ASSERT_DUMMY::Crash(nullptr, "prevFadeOut == 0 && fadeIn == 0",
                "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h", 260, empty_string);
    }
    dlFree(this->pData);
}

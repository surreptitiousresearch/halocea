/* ?Release@HALO_CROSSFADE_DSP@@QAAXXZ @0x836C0A60 */
// ?Release@HALO_CROSSFADE_DSP@@QAAXXZ -- 0x836C0A7C
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

void HALO_CROSSFADE_DSP::Release()
{
    int newRefCount = refCount - 1;
    refCount = newRefCount;

    bool ignoreAssert = IGNORE_STRONG_ASSERT;
    if (!IGNORE_STRONG_ASSERT && newRefCount < 0)
    {
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "refCount >= 0",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            433,
            empty_string);
        ignoreAssert = IGNORE_STRONG_ASSERT;
    }

    if (refCount <= 0)
    {
        if (!ignoreAssert && cache.nElem > 0)
        {
            // Debug-only: verify this instance isn't already sitting in the free-list.
            int idx = 0;
            HALO_CROSSFADE_DSP **pData = cache.pData;
            while (*pData != this)
            {
                ++idx;
                ++pData;
                if (idx >= cache.nElem)
                    goto pushBack;
            }
            if (idx != -1)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "ds::FindNonSorted(cache, this) == -1",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    439,
                    empty_string);
        }
    pushBack:
        cache.PushBack(this);
    }
}

// ?Acquire@HALO_CROSSFADE_DSP@@SAPAU1@XZ -- 0x836BC9DC
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// `HALO_CROSSFADE_DSP::cache' -- static free-list of released instances (declared in the
// header, defined once here).
dsVECTOR<HALO_CROSSFADE_DSP *, 8> HALO_CROSSFADE_DSP::cache;

HALO_CROSSFADE_DSP *HALO_CROSSFADE_DSP::Acquire()
{
    HALO_CROSSFADE_DSP *result;

    if (cache.nElem)
    {
        result = cache[0];
        cache.EraseNonSorted(0);
    }
    else
    {
        // DEVIATION: the binary calls the corpus's instrumented `operator new(size, file, line)`
        // overload (allocation-tracking cookie); modeled here as plain `new` -- same allocation
        // effect, without the debug bookkeeping.
        result = new HALO_CROSSFADE_DSP();
    }

    if (!IGNORE_STRONG_ASSERT && result->refCount)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "res->refCount == 0",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            421,
            empty_string);

    ++result->refCount;
    return result;
}

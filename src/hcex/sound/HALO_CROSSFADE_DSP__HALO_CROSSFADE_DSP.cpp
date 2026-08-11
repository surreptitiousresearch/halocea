// ??0HALO_CROSSFADE_DSP@@QAA@XZ -- @0x836BC840
#include <string.h>
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

extern const char *FModErrorDesc(FMOD_RESULT res, bool info); // C++ linkage: ?FModErrorDesc@@YAPBDW4FMOD_RESULT@@_N@Z
extern void _apLog(const char *fmt, ...);
extern "C" void osOutputDebugString(const char *fmt, ...);

HALO_CROSSFADE_DSP::HALO_CROSSFADE_DSP()
{
    startTime = 0;
    endTime = 0;
    refCount = 0;
    dsp = nullptr;
    isFadeIn = false;

    // DEVIATION: the decompiler shows this as an unrolled 11-iteration, 8-byte-at-a-time zero
    // loop over the tail of a local FMOD_DSP_DESCRIPTION (its create/release/reset/read/
    // setposition/numparameters/paramdesc/setparameter/getparameter/config/configwidth/
    // configheight fields) -- semantically a single zero-init of the whole descriptor before
    // the four fields actually used are filled in below.
    FMOD_DSP_DESCRIPTION desc;
    memset(&desc, 0, sizeof(desc));
    desc.userdata = this;
    desc.read = &HALO_CROSSFADE_DSP::ReadCallback;
    desc.release = &HALO_CROSSFADE_DSP::ReleaseCallback;
    strcpy(desc.name, "HCEX_Crossfade");

    FMOD_RESULT res = haloSoundSystem->system->createDSP(
        reinterpret_cast<const FMOD::FMOD_DSP_DESCRIPTION *>(&desc), &dsp); // boundary: global vs FMOD:: desc alias
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "haloSoundSystem->system->createDSP(&desc, &dsp)", 367);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "haloSoundSystem->system->createDSP(&desc, &dsp)", 367);
    }

    res = dsp->setBypass(true);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "dsp->setBypass(true)", 369);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "dsp->setBypass(true)", 369);
    }
}

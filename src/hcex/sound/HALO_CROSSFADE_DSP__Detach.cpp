// ?Detach@HALO_CROSSFADE_DSP@@QAAXXZ -- 0x836B53B0
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"

extern const char *FModErrorDesc(FMOD_RESULT res, bool info); // C++ linkage: ?FModErrorDesc@@YAPBDW4FMOD_RESULT@@_N@Z
extern void _apLog(const char *fmt, ...);
extern "C" void osOutputDebugString(const char *fmt, ...);

void HALO_CROSSFADE_DSP::Detach()
{
    FMOD_RESULT res = dsp->setBypass(true);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "dsp->setBypass(true)", 378);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "dsp->setBypass(true)", 378);
    }

    res = dsp->disconnectAll(true, true);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "dsp->disconnectAll(true, true)", 379);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "dsp->disconnectAll(true, true)", 379);
    }
}

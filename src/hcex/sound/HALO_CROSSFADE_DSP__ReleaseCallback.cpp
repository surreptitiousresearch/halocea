// ?ReleaseCallback@HALO_CROSSFADE_DSP@@SA?AW4FMOD_RESULT@@PAUFMOD_DSP_STATE@@@Z -- 0x836B54AC
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern "C" const char *FModErrorDesc(FMOD_RESULT res, bool info);
extern void _apLog(const char *fmt, ...);
extern "C" void osOutputDebugString(const char *fmt, ...);

FMOD_RESULT __fastcall HALO_CROSSFADE_DSP::ReleaseCallback(FMOD_DSP_STATE *dsp_state)
{
    // DEVIATION: unlike ReadCallback (which extracts dsp_state->instance before calling any
    // FMOD::DSP method), the disassembly here calls getUserData directly on the incoming
    // pointer with no such extraction -- reproduced verbatim via this cast rather than
    // "correcting" it to go through ->instance, since the binary's actual behavior is the
    // ground truth this function must match.
    void *userdata = nullptr;
    FMOD_RESULT res = reinterpret_cast<FMOD::DSP *>(dsp_state)->getUserData(&userdata);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "dsp->getUserData(&userdata)", 452);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "dsp->getUserData(&userdata)", 452);
    }

    HALO_CROSSFADE_DSP *state = static_cast<HALO_CROSSFADE_DSP *>(userdata);
    if (!IGNORE_STRONG_ASSERT && !state->dsp)
        STRONG_ASSERT_DUMMY().Crash(
            "state->dsp != 0",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            456,
            dsStrongAssertMessage);

    state->dsp = nullptr;
    operator delete(state);
    return FMOD_OK;
}

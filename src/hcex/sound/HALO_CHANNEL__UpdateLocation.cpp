#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

extern "C" float sqrtf(float x); // boundary -- CRT/libm (disasm: fsqrts, single-precision)

// HALO_CHANNEL::UpdateLocation(HALO_PERM_SOUND *snd) @ 0x836B9880
// Push props' 3D position/velocity/forward-orientation/occlusion to `snd`'s live FMOD channel
// (only while props.is3D); each vector is finite-checked first (debug build). If `snd->params`
// requests a pan-level remap (HALO_SOUND_HAS_DIST_LT... actually HALO_SOUND_UPDATE_PANLEVEL bit),
// recompute Set3DPanLevel from squared distance-to-listener against params' dist2D/dist3D cutoffs.
void HALO_CHANNEL::UpdateLocation(HALO_PERM_SOUND *snd)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::UpdateLocation", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2197,
                empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && !snd->channel)
        STRONG_ASSERT2_HELPER::asserd(
            "snd.channel != NULL", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            2199, "this->index", this->index);

    if (!this->props.is3D)
        return;

    m3dV posCheck = this->props.position;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&posCheck))
        STRONG_ASSERT2_HELPER::asserd(
            "m3dCheckFinite(__tmp__)", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            2204, "__tmp__", posCheck);

    m3dV velCheck = this->props.velocity;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&velCheck))
        STRONG_ASSERT2_HELPER::asserd(
            "m3dCheckFinite(__tmp__)", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            2205, "__tmp__", velCheck);

    FMOD_VECTOR posVec = { this->props.position.x, this->props.position.y, this->props.position.z };
    FMOD_VECTOR velVec = { this->props.velocity.x, this->props.velocity.y, this->props.velocity.z };
    snd->channel->set3DAttributes(&posVec, &velVec);

    m3dV fwdCheck = this->props.forward;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&fwdCheck))
        STRONG_ASSERT2_HELPER::asserd(
            "m3dCheckFinite(__tmp__)", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            2210, "__tmp__", fwdCheck);

    FMOD_VECTOR fwdVec = { this->props.forward.x, this->props.forward.y, this->props.forward.z };
    snd->channel->set3DConeOrientation(&fwdVec);

    snd->channel->set3DOcclusion(this->props.occlusion, this->props.obstruction);

    const HALO_SOUND_PARAMS *params = snd->params;
    if (params && ((static_cast<unsigned int>(params->state.val) >> 2) & 1) != 0)
    {
        float dist2D = params->dist2D;
        float dist3D = params->dist3D;
        float distSq = m3dLengthVector_2(&this->props.position);

        float panLevel;
        if (distSq < dist3D * dist3D - 0.000001f)
        {
            if (distSq > dist2D * dist2D + 0.000001f)
            {
                _m3dCheckValid(distSq);
                panLevel = (sqrtf(distSq) - dist2D) / (dist3D - dist2D);
            }
            else
            {
                panLevel = 0.0f;
            }
        }
        else
        {
            panLevel = 1.0f;
        }

        snd->channel->set3DPanLevel(panLevel);
    }
}

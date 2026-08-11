#include "../headers/hcex/HALO_CHANNEL_boundary.h"

// HALO_CHANNEL::UpdateLocation @ 0x836B9838 (private) — push props' 3D position/velocity/
// orientation/occlusion to `snd`'s live FMOD channel (only when props.is3D), plus a pan-level
// remap once the sound's params carry HALO_SOUND_UPDATE_PANLEVEL distance settings.
void HALO_CHANNEL::UpdateLocation(HALO_PERM_SOUND *snd)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::UpdateLocation", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2197, empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && !snd->channel)
        STRONG_ASSERT2_HELPER::asserd<int>("snd.channel != NULL",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2199,
            "this->index", this->index);

    if (!this->props.is3D)
        return;

    m3dV tmp;

    tmp = this->props.position;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&tmp))
        STRONG_ASSERT2_HELPER::asserd<m3dV>("m3dCheckFinite(__tmp__)",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2204, "__tmp__", tmp);

    tmp = this->props.velocity;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&tmp))
        STRONG_ASSERT2_HELPER::asserd<m3dV>("m3dCheckFinite(__tmp__)",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2205, "__tmp__", tmp);

    FMOD_VECTOR pos = { this->props.position.x, this->props.position.y, this->props.position.z };
    FMOD_VECTOR vel = { this->props.velocity.x, this->props.velocity.y, this->props.velocity.z };
    snd->channel->set3DAttributes(&pos, &vel);

    tmp = this->props.forward;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&tmp))
        STRONG_ASSERT2_HELPER::asserd<m3dV>("m3dCheckFinite(__tmp__)",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2210, "__tmp__", tmp);

    FMOD_VECTOR orientation = { this->props.forward.x, this->props.forward.y, this->props.forward.z };
    snd->channel->set3DConeOrientation(&orientation);
    snd->channel->set3DOcclusion(this->props.occlusion, this->props.obstruction);

    const HALO_SOUND_PARAMS *params = snd->params;
    if (params && (((unsigned int)params->state.val >> 2) & 1) != 0)
    {
        float dist2D = params->dist2D;
        float dist3D = params->dist3D;
        float lenSq = m3dLengthVector_2(&this->props.position);

        float panLevel;
        if (lenSq < (float)((float)(dist3D * dist3D) - (float)0.000001))
        {
            if (lenSq > (float)((float)(dist2D * dist2D) + (float)0.000001))
            {
                _m3dCheckValid(lenSq);
                panLevel = (float)((float)(sqrtf(lenSq) - dist2D) / (float)(dist3D - dist2D));
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

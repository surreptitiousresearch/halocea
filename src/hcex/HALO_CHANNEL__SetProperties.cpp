#include "../headers/hcex/HALO_CHANNEL_boundary.h"

// HALO_CHANNEL::SetProperties @ 0x836BD4F8 — update props' volume/pitch/cone/attenuation fields
// from `properties` (skipping everything but volume/gain when gainOnly), recompute
// distanceToPlayer, then push to whichever of current/queued has a live FMOD channel via
// UpdateProperties(initial=false).
//
// `class_index` (3rd mangled arg, ?...@@QAAX...PBU...@@_NH@Z) is unreferenced by the body.
void HALO_CHANNEL::SetProperties(const platform_sound_channel_properties *properties,
                                 bool gainOnly, int class_index)
{
    (void)class_index;
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::SetProperties", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2084, empty_string);
        return;
    }

    this->props.volume = properties->gain;
    if (!gainOnly)
    {
        this->props.pitch = properties->pitch;
        this->props.minDist = properties->minimum_distance;
        this->props.maxDist = properties->maximum_distance;
        this->props.innerConeAngle = properties->inner_cone_angle;
        this->props.outerConeAngle = properties->outer_cone_angle;
        this->props.outerConeGain = properties->outer_cone_gain;
        this->props.distanceToPlayer = m3dLengthVector(&this->props.position);
    }

    if (this->current.channel)
        UpdateProperties(&this->current, false);
    if (this->queued.channel)
        UpdateProperties(&this->queued, false);
}

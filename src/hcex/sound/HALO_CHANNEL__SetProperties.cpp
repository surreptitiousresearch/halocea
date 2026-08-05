#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/platform_sound_channel_properties.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_CHANNEL::SetProperties(const platform_sound_channel_properties *properties,
//                              bool gainOnly, int class_index) @ 0x836BD538
// Update props' volume (always) and, unless gainOnly, pitch/cone/attenuation fields plus
// distanceToPlayer (recomputed from props.position); then push to whichever of current/queued
// has a live FMOD channel via UpdateProperties(initial=false). `class_index` is accepted for
// the DB's declared signature but not read by this body.
void HALO_CHANNEL::SetProperties(const platform_sound_channel_properties *properties, bool gainOnly,
                                    int class_index)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::SetProperties", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2084,
                empty_string);
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
        this->UpdateProperties(&this->current, false);
    if (this->queued.channel)
        this->UpdateProperties(&this->queued, false);
}

#include <stdint.h>
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"
#include "../../headers/hcex/TAG_INFO.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/sound_permutation.h"
#include "headers/math_constants.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary
extern "C" char *tag_get_name(int16_t tag_index); // boundary -- src/tag_get_name.c

// HALO_CHANNEL::UpdateProperties(HALO_PERM_SOUND *snd, bool initial) @ 0x836B9550
// Push props' volume/pitch to `snd`'s live FMOD channel. `initial` (once per Play()) also sets
// the channel's 2D/3D mode, cone settings + min/max distance + spread (3D), or an explicit
// speaker mix for weapon/ambient 2D categories. Volume is muted entirely when
// dbg_mute2dSounds forces 2D sounds off or the debug system is set to skip this tag; otherwise
// it's props.volume scaled by the matched HALO_SOUND_PARAMS' volume (3D) or volume*volume2D
// (2D). Frequency is re-derived each call from the FMOD sound's default frequency / props.pitch.
void HALO_CHANNEL::UpdateProperties(HALO_PERM_SOUND *snd, bool initial)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::UpdateProperties", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2117,
                empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && !snd->channel)
        STRONG_ASSERT2_HELPER::asserd(
            "snd.channel != NULL", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            2119, "this->index", this->index);

    if (initial)
    {
        // Raw FMOD_MODE constants as observed in the decompile (8/16); this FMOD Ex build's
        // FMOD_2D/FMOD_3D bit values differ from the SDK header default (0x4/0x8), so kept
        // literal rather than through fmod_boundary.h's generic macros.
        snd->channel->setMode(this->props.is3D ? 16u : 8u);

        if (this->props.is3D)
        {
            float outerConeGain, outerAngleDeg, innerAngleDeg;
            if (dbg_disable3DCone.value)
            {
                outerConeGain = 1.0f;
                outerAngleDeg = 360.0f;
                innerAngleDeg = 360.0f;
            }
            else
            {
                outerConeGain = this->props.outerConeGain;
                outerAngleDeg = this->props.outerConeAngle * RAD_TO_DEG;
                innerAngleDeg = this->props.innerConeAngle * RAD_TO_DEG;
            }
            snd->channel->set3DConeSettings(innerAngleDeg, outerAngleDeg, outerConeGain);

            float maxDist = 10000.0f;
            if (this->props.maxDist <= 10000.0f)
                maxDist = this->props.maxDist;
            snd->channel->set3DMinMaxDistance(this->props.minDist, maxDist);
            snd->channel->set3DSpread(45.0f);
        }
        else
        {
            unsigned int tagState = snd->tagInfo->state.val;
            if ((tagState & 1) != 0)
            {
                snd->channel->setSpeakerMix(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
            else if (((tagState >> 1) & 1) != 0)
            {
                snd->channel->setSpeakerMix(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
            }
        }
    }

    float finalVolume;
    if (dbg_mute2dSounds.value && !this->props.is3D)
    {
        finalVolume = 0.0f;
    }
    else
    {
        const char *tagName = tag_get_name(snd->info->runtime_tag_index);
        if (snd::SystemFMod->dbg.IsSkipped(tagName))
        {
            finalVolume = 0.0f;
        }
        else
        {
            const HALO_SOUND_PARAMS *params = snd->params;
            if (this->props.is3D)
            {
                float paramVolume = params ? params->volume : 1.0f;
                finalVolume = this->props.volume * paramVolume;
            }
            else
            {
                float paramBaseVolume = params ? params->volume : 1.0f;   // decompile's `v18`
                float paramVolume2D = params ? params->volume2D : 1.0f;
                finalVolume = (this->props.volume * paramVolume2D) * paramBaseVolume;
            }
        }
    }

    snd->channel->setVolume(finalVolume);

    FMOD::Sound *currentSound = nullptr;
    snd->channel->getCurrentSound(&currentSound);
    float defaultFreq = 0.0f;
    if (currentSound->getDefaults(&defaultFreq, nullptr, nullptr, nullptr) == FMOD_OK)
    {
        float newFreq = defaultFreq / this->props.pitch;
        snd->channel->setFrequency(newFreq);
    }
}

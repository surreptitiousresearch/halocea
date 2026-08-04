#include "../headers/hcex/HALO_CHANNEL_boundary.h"
#include "../headers/sound_permutation.h"
#include "headers/math_constants.h"

// HALO_CHANNEL::UpdateProperties @ 0x836B9508 (private) — push props' volume/pitch and (on
// `initial`) 2D/3D mode, cone settings, min/max distance, spread, and explicit dialog/music
// speaker mix to `snd`'s live FMOD channel. `initial` is set once per Play(), not on every
// SetProperties() update.
//
// setMode's argument (8 == FMOD_2D, 16 == FMOD_3D) is reproduced numerically from the decompile.
void HALO_CHANNEL::UpdateProperties(HALO_PERM_SOUND *snd, bool initial)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::UpdateProperties", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2117, &empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && !snd->channel)
        STRONG_ASSERT2_HELPER::asserd<int>("snd.channel != NULL",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2119,
            "this->index", this->index);

    float volume = 1.0f;

    if (initial)
    {
        snd->channel->setMode(!this->props.is3D ? 8 : 16);
        if (this->props.is3D)
        {
            float innerAngle, outerAngle, outerGain;
            if (dbg_disable3DCone.value)
            {
                outerGain = 1.0f;
                outerAngle = 360.0f;
                innerAngle = 360.0f;
            }
            else
            {
                outerGain = this->props.outerConeGain;
                outerAngle = (float)(this->props.outerConeAngle * (float)RAD_TO_DEG);
                innerAngle = (float)(this->props.innerConeAngle * (float)RAD_TO_DEG);
            }
            snd->channel->set3DConeSettings(innerAngle, outerAngle, outerGain);

            float maxDist = 10000.0f;
            if (this->props.maxDist <= 10000.0f)
                maxDist = this->props.maxDist;
            snd->channel->set3DMinMaxDistance(this->props.minDist, maxDist);
            snd->channel->set3DSpread(45.0f);
        }
        else
        {
            // 2D: dialog (bit0) plays centre-only, music (bit1) plays front L/R; others silent.
            unsigned int val = snd->tagInfo->state.val;
            float frontL, frontR;
            bool doMix = false;
            if ((val & 1) != 0)
            {
                frontL = 0.0f;
                frontR = 0.0f;
                doMix = true;
            }
            else if (((val >> 1) & 1) != 0)
            {
                frontL = 1.0f;
                frontR = 1.0f;
                doMix = true;
            }
            if (doMix)
                snd->channel->setSpeakerMix(1.0f, 1.0f, 0.0f, 0.0f,
                                            frontL, frontR, 0.0f, 0.0f);
        }
    }

    const char *name = nullptr;
    if ((dbg_mute2dSounds.value && !this->props.is3D)
        || (name = tag_get_name(snd->info->runtime_tag_index),
            snd::SystemFMod->dbg.IsSkipped(name)))
    {
        volume = 0.0f;
    }
    else
    {
        const HALO_SOUND_PARAMS *params = snd->params;
        if (this->props.is3D)
        {
            float v = 1.0f;
            if (params)
                v = params->volume;
            volume = (float)(this->props.volume * v);
        }
        else
        {
            float base = params ? params->volume : 1.0f;
            float v2d = 1.0f;
            if (params)
                v2d = params->volume2D;
            volume = (float)((float)(this->props.volume * v2d) * base);
        }
    }

    snd->channel->setVolume(volume);

    FMOD::Sound *currentSound = nullptr;
    snd->channel->getCurrentSound(&currentSound);
    float defaultFreq = 0.0f;
    if (currentSound->getDefaults(&defaultFreq, nullptr, nullptr, nullptr) == FMOD_OK)
    {
        snd->channel->setFrequency((float)(defaultFreq / this->props.pitch));
    }
}

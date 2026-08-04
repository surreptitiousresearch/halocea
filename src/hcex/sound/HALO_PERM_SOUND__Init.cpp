// ?Init@HALO_PERM_SOUND@@QAAXPBUsound_permutation@@JABUHALO_CHANNEL_PROPERTIES@@@Z -- 0x836C0C74
#include <stdint.h>
#include <string.h>
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_CHANNEL_PROPERTIES.h"
#include "../../headers/hcex/TAG_INFO.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/sound_permutation.h"

extern "C" unsigned int osGetTime();
extern "C" unsigned int osGetCurThreadId();
extern "C" void         osOutputDebugString(const char *fmt, ...);
extern "C" char *tag_get_name(int16_t tag_index);

void HALO_PERM_SOUND::Init(const sound_permutation *info_, int identifier_,
                            const HALO_CHANNEL_PROPERTIES &channelProps)
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId())
    {
        info = info_;

        if (haloSoundSystem->FindSoundByName(info_, identifier_, channelProps,
                                              &soundList, &fsbIndex, &params))
        {
            const char *name = tag_get_name(info_->runtime_tag_index);
            tagInfo = haloSoundSystem->tagInfoPool.policy.Alloc(name, haloSoundSystem->tagInfoPool.usedNodes);

            if (tagInfo->count == 1)
            {
                // Classify the tag by its path prefix into the TAG_INFO_STATE category bits.
                int stateVal = tagInfo->state.val;
                stateVal = (strncmp(tagInfo->name, "sound\\weapons\\", 14) == 0)  ? (stateVal | TAG_INFO_WEAPON)  : (stateVal & ~TAG_INFO_WEAPON);
                tagInfo->state.val = stateVal;

                stateVal = tagInfo->state.val;
                stateVal = (strncmp(tagInfo->name, "sound\\ambience\\", 15) == 0) ? (stateVal | TAG_INFO_AMBIENT) : (stateVal & ~TAG_INFO_AMBIENT);
                tagInfo->state.val = stateVal;

                stateVal = tagInfo->state.val;
                stateVal = (strncmp(tagInfo->name, "sound\\dialog\\", 13) == 0)   ? (stateVal | TAG_INFO_DIALOG)  : (stateVal & ~TAG_INFO_DIALOG);
                tagInfo->state.val = stateVal;

                stateVal = tagInfo->state.val;
                stateVal = (strncmp(tagInfo->name, "sound\\music\\", 12) == 0)    ? (stateVal | TAG_INFO_MUSIC)   : (stateVal & ~TAG_INFO_MUSIC);
                tagInfo->state.val = stateVal;
            }

            identifier = identifier_;
            initTime = osGetTime();
        }
        else
        {
            soundList = nullptr;
            params = nullptr;
            fsbIndex = -1;
            identifier = -1;
            initTime = osGetTime();
        }
    }
    else
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_PERM_SOUND::Init", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1093,
                dsStrongAssertMessage);
    }
}

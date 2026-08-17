/* ?Stop@HALO_PERM_SOUND@@QAAXXZ @0x836C2210 */
// ?Stop@HALO_PERM_SOUND@@QAAXXZ -- 0x836C2248
#include <stdint.h>
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/hcex/TAG_INFO.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/sound_permutation.h"

// DEVIATION: osGetCurThreadId does not share its neighbours' C linkage — the DB carries only
// `?osGetCurThreadId@@YAHXZ`, a mangled C++ name returning int (src/ws/os/osGetCurThreadId.cpp).
int osGetCurThreadId();
extern "C" void         osOutputDebugString(const char *fmt, ...);
extern "C" char *tag_get_name(int tag_index);
extern void hcex_notify_sound_stop(const char *tag_name);

void HALO_PERM_SOUND::Stop()
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId())
    {
        if (prevFadeOut)
        {
            prevFadeOut->Detach();
            prevFadeOut->Release();
            prevFadeOut = nullptr;
        }
        if (fadeIn)
        {
            fadeIn->Detach();
            fadeIn->Release();
            fadeIn = nullptr;
        }
        if (tagInfo)
        {
            hcex_notify_sound_stop(tagInfo->name);
            haloSoundSystem->tagInfoPool.policy.Delete(tagInfo, haloSoundSystem->tagInfoPool.usedNodes);
            tagInfo = nullptr;
        }
        if (channel)
        {
            bool isPlaying = false;
            channel->isPlaying(&isPlaying);
            if (isPlaying)
                channel->stop();
            channel = nullptr;
        }

        if (isUsed)
        {
            // Find this instance's checked-out slot in haloSoundSystem->preloadList and mark
            // it free again.
            bool found = false;
            int preloadIndex = 0;
            if (haloSoundSystem->preloadList.nElem > 0)
            {
                HALO_PERM_SOUND *preloaded;
                while (true)
                {
                    preloaded = &haloSoundSystem->preloadList[preloadIndex];
                    if (preloaded->info == info)
                        break;
                    if (++preloadIndex >= haloSoundSystem->preloadList.nElem)
                        goto preloadNotFound;
                }
                if (!IGNORE_STRONG_ASSERT && !preloaded->isUsed)
                    static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                        "p.isUsed",
                        "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                        1772,
                        empty_string);
                found = true;
                preloaded->isUsed = false;
            }
        preloadNotFound:
            // DEVIATION: simplified from the decompile's raw STRONG_ASSERT2_HELPER::asserd<char*,
            // char const[32]> call (debug-only diagnostic naming the tag when the preload-list
            // lookup above fails) to a single-value assert -- same trigger condition and message,
            // this cluster does not reverse the exact multi-value asserd overload set.
            if (!IGNORE_STRONG_ASSERT && !found)
                STRONG_ASSERT2_HELPER::asserd(
                    "found",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1780,
                    "tag_get_name(info->runtime_tag_index)",
                    tag_get_name(info->runtime_tag_index));
        }
        else if (soundBank)
        {
            if (!IGNORE_STRONG_ASSERT && !soundList)
                STRONG_ASSERT2_HELPER::asserd(
                    "soundList != NULL",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1756,
                    "tag_get_name(info->runtime_tag_index)",
                    tag_get_name(info->runtime_tag_index));
            soundList->ReleaseSound(soundBank);
            soundBank = nullptr;
        }

        fadeIn = nullptr;
        prevFadeOut = nullptr;
        soundBank = nullptr;
        identifier = -1;
        tagInfo = nullptr;
        params = nullptr;
        previousFreq = 0.0f;
        wasUsed = false;
        isUsed = false;
        initTime = 0;
        previousChannel = nullptr;
        channel = nullptr;
        subSound = nullptr;
        fsbIndex = -1;
        soundList = nullptr;
        info = nullptr;
    }
    else
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_PERM_SOUND::Stop", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1733,
                empty_string);
    }
}

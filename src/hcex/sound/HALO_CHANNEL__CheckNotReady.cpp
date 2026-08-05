#include <stdint.h>
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"
#include "../../headers/hcex/TAG_INFO.h"
#include "../../headers/sound_permutation.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary
extern "C" char *tag_get_name(int16_t tag_index); // boundary -- src/tag_get_name.c

// HALO_CHANNEL::CheckNotReady(HALO_PERM_SOUND *sound) @ 0x836C2C6C
// Two paths:
//  - `sound` already has an FMOD channel (fsbIndex/soundBank/channel all set): this is the
//    "already playing, maybe re-syncing after a stitch" case -- if there's a previousChannel
//    (loop-point stitch) and its frequency has drifted from what was captured at stitch time,
//    pause/UpdateStitchingDelay/unpause to resync.
//  - otherwise: wait for soundBank to finish opening, resolve the target sub-sound
//    (getSubSound), wait for IT to finish opening too (returning true = "still not ready" to
//    either path), then actually start playback: steal an old channel if FMOD is at its
//    32-channel cap, playSound (paused), route into the reverb or master ChannelGroup
//    (reverb only when tagInfo requests it and this channel isn't itself 3D / dbg_disableEAX
//    isn't forcing master), push initial location+properties, unpause, notify the outer engine
//    the sound started, and fire the debug "PLAY ..." log line.
bool HALO_CHANNEL::CheckNotReady(HALO_PERM_SOUND *sound)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::CheckNotReady", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1874,
                empty_string);
        return false;
    }

    if (sound->fsbIndex != -1 && sound->soundBank != nullptr && sound->channel != nullptr)
    {
        // Already playing -- only re-sync a stitched loop point's playback frequency.
        FMOD::Channel *previousChannel = sound->previousChannel;
        if (previousChannel)
        {
            float freq = 0.0f;
            if (previousChannel->getFrequency(&freq) == FMOD_OK
                && freq >= 0.000001f && freq != sound->previousFreq)
            {
                sound->channel->setPaused(true);
                sound->UpdateStitchingDelay(true);
                sound->channel->setPaused(haloSoundSystem->isPaused);
            }
        }
        return false;
    }

    if (!sound->subSound)
    {
        FMOD_OPENSTATE openState = FMOD_OPENSTATE_READY;
        FMOD_RESULT result = sound->soundBank->getOpenState(&openState, nullptr, nullptr);
        if (result != FMOD_OK)
        {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"sound.soundBank->getOpenState(&openState, NULL, NULL)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1902,
                    empty_string);
            const char *desc = FModErrorDesc(result, false);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                 1902, result, desc);
        }
        if (openState != FMOD_OPENSTATE_READY)
            return true;

        int subSoundCount = 0;
        result = sound->soundBank->getNumSubSounds(&subSoundCount);
        if (result != FMOD_OK)
        {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"sound.soundBank->getNumSubSounds(&count)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1908,
                    empty_string);
            const char *desc = FModErrorDesc(result, false);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                 1908, result, desc);
        }

        if (!IGNORE_STRONG_ASSERT && subSoundCount != sound->soundList->soundsCount)
            STRONG_ASSERT2_HELPER::asserd(
                "count == sound.soundList->soundsCount || !\"Old sound banks\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1909,
                "count", subSoundCount, "sound.soundList->tags.Length()", sound->soundList->tags.nElem,
                "sound.soundList->fileName", sound->soundList->fileName);

        result = sound->soundBank->getSubSound(sound->fsbIndex, &sound->subSound);
        if (result != FMOD_OK)
        {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"sound.soundBank->getSubSound(sound.fsbIndex, &sound.subSound)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1911,
                    empty_string);
            const char *desc = FModErrorDesc(result, false);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                 1911, result, desc);
        }
    }

    FMOD_OPENSTATE subOpenState = FMOD_OPENSTATE_READY;
    FMOD_RESULT result = sound->subSound->getOpenState(&subOpenState, nullptr, nullptr);
    if (result != FMOD_OK)
    {
        if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                "\"sound.subSound->getOpenState(&openState, NULL, NULL)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1916,
                empty_string);
        const char *desc = FModErrorDesc(result, false);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                             "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                             1916, result, desc);
    }
    if (subOpenState != FMOD_OPENSTATE_READY)
        return true;

    int channelsPlaying = 0;
    haloSoundSystem->system->getChannelsPlaying(&channelsPlaying);
    if (channelsPlaying + 1 >= 32)
        haloSoundSystem->StealOldChannel();

    result = haloSoundSystem->system->playSound(FMOD_CHANNEL_FREE, sound->subSound, true, &sound->channel);
    if (result != FMOD_OK)
    {
        if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                "\"haloSoundSystem->system->playSound(FMOD_CHANNEL_FREE, sound.subSound, true, "
                "&sound.channel)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1928,
                empty_string);
        const char *desc = FModErrorDesc(result, false);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                             "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                             1928, result, desc);
    }

    sound->UpdateStitchingDelay(false);

    FMOD::ChannelGroup *channelGroup = snd::SystemFMod->master;
    unsigned int tagState = sound->tagInfo->state.val;
    if ((tagState & 0xA) == 0 && (((tagState >> 2) & 1) == 0 || this->props.is3D))
        channelGroup = snd::SystemFMod->reverb;
    if (dbg_disableEAX.value)
        channelGroup = snd::SystemFMod->master;

    result = sound->channel->setChannelGroup(channelGroup);
    if (result != FMOD_OK)
    {
        if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                "\"sound.channel->setChannelGroup(channelGroup)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1942,
                empty_string);
        const char *desc = FModErrorDesc(result, false);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                             "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                             1942, result, desc);
    }

    this->UpdateProperties(sound, true);
    this->UpdateLocation(sound);
    sound->channel->setPaused(haloSoundSystem->isPaused);
    hcex_notify_sound_start(sound->tagInfo->name);

    dsTSTRING<char> idSuffix;
    idSuffix.pBuffer = nullptr;
    bool hasIdSuffix = false;
    if (sound->identifier != -1)
    {
        hasIdSuffix = true;
        const char *idName = tag_get_name(sound->identifier);
        dsSPrintf(&idSuffix, " [%s]", idName);
    }

    dsTSTRING<char> slash;
    slash.pBuffer = nullptr;
    slash.UnsafeInit("\\", -1, 0);

    dsTSTRING<char> tagPathAndSlash;
    tagPathAndSlash.pBuffer = nullptr;
    const char *tagPath = tag_get_name(sound->info->runtime_tag_index);
    dsTSTRING_concat(&tagPathAndSlash, tagPath, &slash);

    dsTSTRING<char> fullTagName;
    fullTagName.pBuffer = nullptr;
    dsTSTRING_concat(&fullTagName, &tagPathAndSlash, sound->info->name);

    dsTSTRING<char> logLine;
    logLine.pBuffer = nullptr;
    dsSPrintf(&logLine, "PLAY %s%s\n", fullTagName.pBuffer->str, hasIdSuffix ? idSuffix.pBuffer->str : "");
    if (dbg_printBufPlay_10.value)
        osOutputDebugString("%s", logLine.pBuffer->str);

    return false;
}

#include "../headers/hcex/HALO_CHANNEL_boundary.h"
#include "../headers/sound_permutation.h"

// HALO_CHANNEL::CheckNotReady @ 0x836C2C50 (private) — finish standing up `sound`'s FMOD
// playback. If its channel already exists, only re-sync playback frequency after a stitch
// (loop-point) change; otherwise wait for the sound bank to finish opening, resolve its
// sub-sound, start FMOD::System::playSound (stealing an old channel first if FMOD is at its
// 32-channel cap), assign the channel group (master/reverb), push initial location+properties,
// and fire the debug "PLAY ..." log line. Returns true while still waiting on the bank/sub-sound
// to open (caller should retry next Update()).
//
// The raw decompile's inlined operator+<char> string concatenations are reproduced via the
// cluster's named dsTSTRING_concat helpers (see HALO_SOUND_LIST_HALO_CHANNEL_globals.h). Stray
// per-call FMOD error-report blocks are the STRONG_ASSERT + "%s(%d): FMOD error ..." idiom.
bool HALO_CHANNEL::CheckNotReady(HALO_PERM_SOUND *sound)
{
    dsTSTRING<char> idSuffix;
    idSuffix.pBuffer = nullptr;

    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::CheckNotReady", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1874, &empty_string);
        return false;
    }

    // Already have (or can't yet get) a channel: only handle post-stitch frequency re-sync.
    if (sound->fsbIndex == -1 || sound->soundBank == nullptr || sound->channel != nullptr)
    {
        FMOD::Channel *previousChannel = sound->previousChannel;
        if (previousChannel)
        {
            float prevFreq = 0.0f;
            if (previousChannel->getFrequency(&prevFreq) == FMOD_OK
                && prevFreq >= 0.000001f
                && prevFreq != sound->previousFreq)
            {
                sound->channel->setPaused(true);
                sound->UpdateStitchingDelay(true);
                sound->channel->setPaused(haloSoundSystem->isPaused);
            }
        }
        return false;
    }

    // Open + play path: bank is present, channel not yet created.
    const char *fileNotFound = "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp";

    if (!sound->subSound)
    {
        FMOD_OPENSTATE openState = FMOD_OPENSTATE_READY;
        FMOD_RESULT r = sound->soundBank->getOpenState(&openState, nullptr, nullptr);
        if (r)
        {
            if (r == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.soundBank->getOpenState(&openState, NULL, NULL)\"",
                    fileNotFound, 1902, &empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", fileNotFound, 1902, r, FModErrorDesc(r, false));
        }
        if (openState != FMOD_OPENSTATE_READY)
            return true;

        int subSoundCount = 0;
        r = sound->soundBank->getNumSubSounds(&subSoundCount);
        if (r)
        {
            if (r == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.soundBank->getNumSubSounds(&count)\"",
                    fileNotFound, 1908, &empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", fileNotFound, 1908, r, FModErrorDesc(r, false));
        }

        if (!IGNORE_STRONG_ASSERT)
        {
            HALO_SOUND_LIST *soundList = sound->soundList;
            if (subSoundCount != soundList->soundsCount)
            {
                int tagsLen = soundList->tags.nElem;
                STRONG_ASSERT2_HELPER::asserd<int, int, dsTSTRING<char>>(
                    "count == sound.soundList->soundsCount || !\"Old sound banks\"",
                    fileNotFound, 1909,
                    "count", subSoundCount,
                    "sound.soundList->tags.Length()", tagsLen,
                    "sound.soundList->fileName", soundList->fileName);
            }
        }

        r = sound->soundBank->getSubSound(sound->fsbIndex, &sound->subSound);
        if (r)
        {
            if (r == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.soundBank->getSubSound(sound.fsbIndex, &sound.subSound)\"",
                    fileNotFound, 1911, &empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", fileNotFound, 1911, r, FModErrorDesc(r, false));
        }
    }

    FMOD_OPENSTATE subOpenState = FMOD_OPENSTATE_READY;
    FMOD_RESULT r = sound->subSound->getOpenState(&subOpenState, nullptr, nullptr);
    if (r)
    {
        if (r == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.subSound->getOpenState(&openState, NULL, NULL)\"",
                fileNotFound, 1916, &empty_string);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", fileNotFound, 1916, r, FModErrorDesc(r, false));
    }
    if (subOpenState != FMOD_OPENSTATE_READY)
        return true;

    int channelsPlaying = 0;
    haloSoundSystem->system->getChannelsPlaying(&channelsPlaying);
    if (channelsPlaying + 1 >= 32)
        haloSoundSystem->StealOldChannel();

    r = haloSoundSystem->system->playSound(FMOD_CHANNEL_FREE, sound->subSound, true, &sound->channel);
    if (r)
    {
        if (r == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"haloSoundSystem->system->playSound(FMOD_CHANNEL_FREE, sound.subSound, true, &sound.channel)\"",
                fileNotFound, 1928, &empty_string);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", fileNotFound, 1928, r, FModErrorDesc(r, false));
    }

    sound->UpdateStitchingDelay(false);

    // Choose the mixer bus: reverb-send unless the sound is dialog/music (bits 1,3) or a
    // 2D pan-level sound, or EAX is force-disabled.
    FMOD::ChannelGroup *channelGroup = snd::SystemFMod->master;
    unsigned int val = sound->tagInfo->state.val;
    if ((val & 0xA) == 0 && (((val >> 2) & 1) == 0 || this->props.is3D))
        channelGroup = snd::SystemFMod->reverb;
    if (dbg_disableEAX.value)
        channelGroup = snd::SystemFMod->master;

    r = sound->channel->setChannelGroup(channelGroup);
    if (r)
    {
        if (r == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setChannelGroup(channelGroup)\"",
                fileNotFound, 1942, &empty_string);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", fileNotFound, 1942, r, FModErrorDesc(r, false));
    }

    UpdateProperties(sound, true);
    UpdateLocation(sound);
    sound->channel->setPaused(haloSoundSystem->isPaused);
    hcex_notify_sound_start(sound->tagInfo->name);

    // Assemble the debug "PLAY <tag>\<perm>[ [id]]" line.
    bool haveSuffix = false;
    const char *suffix = &empty_string;
    if (sound->identifier != -1)
    {
        haveSuffix = true;
        suffix = dsSPrintf(&idSuffix, " [%s]", tag_get_name(sound->identifier))->pBuffer->str;
    }

    dsTSTRING<char> sep;
    sep.pBuffer = nullptr;
    sep.UnsafeInit("\\", -1, 0);

    const sound_permutation *info = sound->info;
    const char *tagName = tag_get_name(sound->info->runtime_tag_index);

    dsTSTRING<char> namePlusSep;
    dsTSTRING<char> fullName;
    dsTSTRING<char> playLine;
    const dsTSTRING<char> *namePlusSepStr = dsTSTRING_concat<char>(&namePlusSep, tagName, &sep);
    const dsTSTRING<char> *fullNameStr = dsTSTRING_concat<char>(&fullName, namePlusSepStr, info->name);

    bool print = dbg_printBufPlay_10.value;
    const dsTSTRING<char> *line = dsSPrintf(&playLine, "PLAY %s%s\n", fullNameStr->pBuffer->str, suffix);
    if (print)
        osOutputDebugString("%s", line->pBuffer->str);

    if (--playLine.pBuffer->refCount == 0)
        dlFree(playLine.pBuffer);
    if (--fullName.pBuffer->refCount == 0)
        dlFree(fullName.pBuffer);
    if (--namePlusSep.pBuffer->refCount == 0)
        dlFree(namePlusSep.pBuffer);
    if (sep.pBuffer->refCount-- == 1)
        dlFree(sep.pBuffer);
    if (haveSuffix)
    {
        if (--idSuffix.pBuffer->refCount == 0)
            dlFree(idSuffix.pBuffer);
    }

    return false;
}

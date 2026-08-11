#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../headers/ws/snd/snd_fmod_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

#define SND_SRC "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp"

// 0x836BE6B8 — advance one debug/custom sound's little state machine: once its streamed bank has
// finished opening, resolve the sub-sound, start it playing (2D, master gain, unpaused), then
// each subsequent call just checks whether it is still playing. When playback ends, release the
// bank back to its HALO_SOUND_LIST and clear soundList. Returns true while the sound is still
// alive (keep it in customSounds), false once finished (caller removes it).
bool HALO_SOUND_SYSTEM::UpdateCustomSound(HALO_CUSTOM_SOUND &sound)
{
    if (!sound.soundList)
        return false;

    if (!sound.subSound)
    {
        FMOD_OPENSTATE openstate = FMOD_OPENSTATE_READY;
        sound.soundBank->getOpenState(&openstate, nullptr, nullptr);
        if (openstate)
            return true; // bank still opening; retry next tick

        FMOD_RESULT res = sound.soundBank->getSubSound(sound.fsbIndex, &sound.subSound);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.soundBank->getSubSound(sound.fsbIndex, &sound.subSound)\"",
                    SND_SRC, 1036, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", SND_SRC, 1036, res, FModErrorDesc(res, false));
        }
        if (!IGNORE_STRONG_ASSERT && !sound.subSound)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("sound.subSound != 0", SND_SRC, 1037, empty_string);
    }

    if (!sound.channel)
    {
        FMOD_OPENSTATE openstate = FMOD_OPENSTATE_READY;
        sound.subSound->getOpenState(&openstate, nullptr, nullptr);
        if (openstate)
            return true; // sub-sound still opening; retry next tick

        FMOD_RESULT res = this->system->playSound(FMOD_CHANNEL_FREE, sound.subSound, true, &sound.channel);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->playSound(FMOD_CHANNEL_FREE, sound.subSound, true, &sound.channel)\"",
                    SND_SRC, 1048, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", SND_SRC, 1048, res, FModErrorDesc(res, false));
        }
        if (!IGNORE_STRONG_ASSERT && !sound.channel)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("sound.channel != 0", SND_SRC, 1049, empty_string);

        res = sound.channel->setMode(FMOD_2D); // li r4, 8 @0x836BE8BC
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setMode(FMOD_2D)\"",
                    SND_SRC, 1051, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", SND_SRC, 1051, res, FModErrorDesc(res, false));
        }

        res = sound.channel->setVolume(sound_manager_globals.master_gain);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setVolume(sound_manager_globals.master_gain)\"",
                    SND_SRC, 1052, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", SND_SRC, 1052, res, FModErrorDesc(res, false));
        }

        res = sound.channel->setPaused(false);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setPaused(false)\"",
                    SND_SRC, 1054, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n", SND_SRC, 1054, res, FModErrorDesc(res, false));
        }
    }

    if (!sound.channel)
        return true;

    bool playing = false;
    sound.channel->isPlaying(&playing);
    if (playing)
        return true;

    sound.soundList->ReleaseSound(sound.soundBank);
    sound.soundList = nullptr;
    return false;
}

#undef SND_SRC

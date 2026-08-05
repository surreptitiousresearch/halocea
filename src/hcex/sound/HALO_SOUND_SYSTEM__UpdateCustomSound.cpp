#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_SOUND_SYSTEM::UpdateCustomSound(HALO_CUSTOM_SOUND&) @ 0x836BE6C4
// Advances one filename-based custom sound through FMOD's async-load state machine: waits for
// the FSB bank to finish opening, then the extracted sub-sound, then starts playback. Returns
// true while the slot is still in flight or actively playing (keep it alive); false once the
// sound has finished playing (release it and let the caller drop the slot).
bool HALO_SOUND_SYSTEM::UpdateCustomSound(HALO_CUSTOM_SOUND &sound)
{
    if (!sound.soundList)
        return false;

    if (!sound.subSound) {
        FMOD_OPENSTATE state = FMOD_OPENSTATE_READY;
        sound.soundBank->getOpenState(&state, nullptr, nullptr);
        if (state)
            return true;

        FMOD_RESULT result = sound.soundBank->getSubSound(sound.fsbIndex, &sound.subSound);
        if (result) {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.soundBank->getSubSound(sound.fsbIndex, &sound.subSound)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1036, empty_string);
            const char *desc = FModErrorDesc(result, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1036, result, desc);
        }
        if (!IGNORE_STRONG_ASSERT && !sound.subSound)
            STRONG_ASSERT_DUMMY().Crash("sound.subSound != 0",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         1037, empty_string);
    }

    if (!sound.channel) {
        FMOD_OPENSTATE state = FMOD_OPENSTATE_READY;
        sound.subSound->getOpenState(&state, nullptr, nullptr);
        if (state)
            return true;

        FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, sound.subSound, true, &sound.channel);
        if (result) {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->playSound(FMOD_CHANNEL_FREE, sound.subSound, true, &sound.channel)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1048, empty_string);
            const char *desc = FModErrorDesc(result, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1048, result, desc);
        }
        if (!IGNORE_STRONG_ASSERT && !sound.channel)
            STRONG_ASSERT_DUMMY().Crash("sound.channel != 0",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         1049, empty_string);

        FMOD_RESULT modeResult = sound.channel->setMode(FMOD_2D);
        if (modeResult) {
            if (modeResult == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setMode(FMOD_2D)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1051, empty_string);
            const char *desc = FModErrorDesc(modeResult, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1051, modeResult, desc);
        }

        FMOD_RESULT volResult = sound.channel->setVolume(sound_manager_globals.master_gain);
        if (volResult) {
            if (volResult == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setVolume(sound_manager_globals.master_gain)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1052, empty_string);
            const char *desc = FModErrorDesc(volResult, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1052, volResult, desc);
        }

        FMOD_RESULT pauseResult = sound.channel->setPaused(false);
        if (pauseResult) {
            if (pauseResult == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound.channel->setPaused(false)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1054, empty_string);
            const char *desc = FModErrorDesc(pauseResult, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1054, pauseResult, desc);
        }
    }

    if (!sound.channel)
        return true;

    bool isPlaying = false;
    sound.channel->isPlaying(&isPlaying);
    if (isPlaying)
        return true;

    sound.soundList->ReleaseSound(sound.soundBank);
    sound.soundList = nullptr;
    return false;
}

#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836B6958 — ?CreateSound@HALO_SOUND_LIST@@AAAPAVSound@FMOD@@XZ
// Open (or create) one FMOD::Sound. isMemory calls FMOD::System::createSound over the resident
// `data`/`dataLength` blob (FMOD_OPENMEMORY_POINT | FMOD_CREATECOMPRESSEDSAMPLE added); otherwise
// FMOD::System::createStream over `fileName`. Both are opened FMOD_NONBLOCKING unless the sound
// system's blocking bit (state bit 9) or the dbg-console disableNonblocking toggle forces a
// blocking open. Any FMOD error is logged (a missing-file error additionally strong-asserts).
//
// DEVIATION: decompiler-invented spill args (a2..a6) dropped; CreateSound takes no parameters.
FMOD::Sound *HALO_SOUND_LIST::CreateSound()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_SOUND_LIST::CreateSound", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1580, empty_string);
        return nullptr;
    }

    // DEVIATION: the low byte is 0x48, not 0x08 -- bit 0x40 (FMOD_SOFTWARE) was dropped from BOTH
    // paths. The binary builds the mode with `lis r5,0xA01 / ori r5,r5,0x48` @0x836B6A10 and the
    // forced-blocking variant with `lis r5,0xA00 / ori r5,r5,0x48` @0x836B6A38 -- 0x0A010048 and
    // 0x0A000048. Every sound was being opened without the software-mixing bit.
    // Found by the linkdup drain: this file is the one being KEPT, and the duplicate scheduled for
    // deletion carried the correct literals. Deleting on the address signal alone would have
    // destroyed the only right copy of this constant.
    unsigned int flags = 0x0A010048;
    if (((unsigned int)snd::SystemFMod->state.val >> 9) & 1 || snd::SystemFMod->dbg.disableNonblocking)
        flags &= ~FMOD_NONBLOCKING; // -> 0x0A000048 (blocking open forced)

    FMOD_CREATESOUNDEXINFO exinfo = {};
    exinfo.cbsize = 112;
    exinfo.suggestedsoundtype = FMOD_SOUND_TYPE_FSB;

    FMOD::Sound *sound = nullptr;
    FMOD_RESULT result;
    if (this->isMemory) {
        exinfo.length = this->dataLength;
        result = haloSoundSystem->system->createSound(
            (const char *)this->data, flags | (FMOD_OPENMEMORY_POINT | FMOD_CREATECOMPRESSEDSAMPLE),
            &exinfo, &sound);
        if (result) {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(
                    nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"haloSoundSystem->system->createSound((char *)data, FMOD_OPENMEMORY_POINT | "
                    "FMOD_CREATECOMPRESSEDSAMPLE | flags, &exinfo, &sound)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1600, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                1600, result, FModErrorDesc(result, false));
        }
    } else {
        result = haloSoundSystem->system->createStream(this->fileName.pBuffer->str, flags, &exinfo, &sound);
        if (result) {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(
                    nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"haloSoundSystem->system->createStream(fileName.CStr(), flags, &exinfo, &sound)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1605, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                1605, result, FModErrorDesc(result, false));
        }
    }
    return sound;
}

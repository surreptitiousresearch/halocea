#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_SOUND_LIST::CreateSound() @ 0x836B6998
//
// CAVEAT: the decompiler mis-parses this as taking 5 extra int/__int64 args -- the disasm
// prologue only reads r3 (`this`); the phantom trailing params are decompiler noise from two
// adjacent local buffers (a scratch dsTSTRING<char>-sized region and the FMOD_CREATESOUNDEXINFO
// struct being zero-filled in a 14-QWORD loop). DB confirms the real 0-arg prototype
// (`FMOD::Sound *__fastcall HALO_SOUND_LIST__CreateSound(HALO_SOUND_LIST*)`).
//
// Open one FMOD::Sound for this list: isMemory creates an in-memory sample from `data`/
// `dataLength` (FMOD_OPENMEMORY_POINT | FMOD_CREATECOMPRESSEDSAMPLE, suggested type FSB);
// otherwise opens `fileName` as a stream. Both add FMOD_NONBLOCKING unless the debug system
// forces a blocking open (snd::SystemFMod->state bit 9, or dbg.disableNonblocking). Off the
// audio thread, this is a no-op returning NULL (strong-assert guarded).
FMOD::Sound *HALO_SOUND_LIST::CreateSound()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_LIST::CreateSound", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1580,
                dsStrongAssertMessage);
        return nullptr;
    }

    // 0xA010048 (includes FMOD_NONBLOCKING) unless the debug system forces blocking, in which
    // case 0xA000048 (FMOD_NONBLOCKING bit cleared) -- matches the decompile's raw constants.
    unsigned int mode = 0xA010048u;
    if (((static_cast<unsigned int>(snd::SystemFMod->state.val) >> 9) & 1) != 0
        || snd::SystemFMod->dbg.disableNonblocking)
        mode = 0xA000048u;

    FMOD_CREATESOUNDEXINFO exinfo = {};
    exinfo.cbsize = 112;
    exinfo.suggestedsoundtype = FMOD_SOUND_TYPE_FSB;

    FMOD::Sound *sound = nullptr;
    FMOD_RESULT result;
    if (this->isMemory)
    {
        exinfo.length = this->dataLength;
        result = haloSoundSystem->system->createSound(
            reinterpret_cast<const char *>(this->data),
            mode | FMOD_OPENMEMORY_POINT | FMOD_CREATECOMPRESSEDSAMPLE,
            reinterpret_cast<FMOD_CREATESOUNDEXINFO *>(&exinfo), &sound); // boundary: global vs FMOD:: exinfo alias
        if (result != FMOD_OK)
        {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"haloSoundSystem->system->createSound((char *)data, FMOD_OPENMEMORY_POINT | "
                    "FMOD_CREATECOMPRESSEDSAMPLE | flags, &exinfo, &sound)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1600,
                    dsStrongAssertMessage);
            const char *desc = FModErrorDesc(result, false);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                 1600, result, desc);
        }
    }
    else
    {
        result = haloSoundSystem->system->createStream(this->fileName.pBuffer->str, mode,
            reinterpret_cast<FMOD_CREATESOUNDEXINFO *>(&exinfo), &sound); // boundary alias
        if (result != FMOD_OK)
        {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
                    "\"haloSoundSystem->system->createStream(fileName.CStr(), flags, &exinfo, &sound)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1605,
                    dsStrongAssertMessage);
            const char *desc = FModErrorDesc(result, false);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                 1605, result, desc);
        }
    }

    return sound;
}

// WaitSoundBank @0x836B5808 — block until a streaming FMOD sound has finished opening. Polls
// FMOD::Sound::getOpenState, asserting/logging on error, and spins (osSleep(0)) while the open state
// is still non-ready. FMOD is an external SDK boundary. See headers/ws/snd/fmod_error_boundary.h.
#include "../headers/ws/snd/fmod_error_boundary.h"

void WaitSoundBank(FMOD::Sound *sound)
{
    // DEVIATION: the ctor parameter is `bool isStarted`, not an initial tick count
    // (??0osTIMER@@QAA@_N@Z; `li r4, 1` at 0x836B5818). Armed for timing/telemetry; result unused.
    osTIMER timer(true);
    // DEVIATION: getOpenState takes THREE parameters here
    // (?getOpenState@Sound@FMOD@@QAA?AW4FMOD_RESULT@@PAW4FMOD_OPENSTATE@@PAIPA_N@Z, and only
    // r4/r5/r6 are set at 0x836B5834); the fourth nullptr this file used to pass was from the
    // later FMOD Ex spelling of the SDK method.
    FMOD_OPENSTATE openstate;
    FMOD_RESULT res;

    res = sound->getOpenState(&openstate, nullptr, nullptr);
    if (res)
    {
        if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound->getOpenState(&state, NULL, NULL)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1142, empty_string);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1142, res, FModErrorDesc(res, false));
    }

    while (openstate)                 // FMOD_OPENSTATE_READY == 0
    {
        osSleep(0);
        res = sound->getOpenState(&openstate, nullptr, nullptr);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"sound->getOpenState(&state, NULL, NULL)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1145, empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1145, res, FModErrorDesc(res, false));
        }
    }
}

#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"

#include "../../headers/ws/ds/ds_assert_boundary.h" // STRONG_ASSERT_DUMMY + empty_string
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */

extern "C" int strcmp(const char *a, const char *b); // boundary — CRT

// dsSPrintf(dsTSTRING<char>&, const char*, ...) @ 0x82790400 — sprintf into a dsTSTRING (sret).
// Boundary — generic ds string-formatting helper, not reversed here.
extern dsTSTRING<char> dsSPrintf(dsTSTRING<char> *result, const char *fmt, ...);

// operator+(dsTSTRING<char>&, const char*, const dsTSTRING<char>&) @ sret concat helper.
// Boundary — generic dsTSTRING concatenation operator, not reversed here.
extern dsTSTRING<char> *operator_plus_dsTSTRING(dsTSTRING<char> *result, const char *lhs, const dsTSTRING<char> &rhs);

// HALO_SOUND_SYSTEM::Init(snd::INIT) @ 0x836C523C
// One-time bring-up: grabs the shared FMOD System object, reads its DSP buffer size (doubled,
// stored as `minDelay`) and software sample rate, pre-grows the channel/sound-list vectors,
// loads the tag-name playback parameter table, then loads all 15 FSB sound-lists named from
// SOUND_LIST_LST, using a locale-suffixed filename ("<name>_<locale>.fsb") whenever the active
// locale isn't "en".
bool HALO_SOUND_SYSTEM::Init(snd::INIT init)
{
    dsTSTRING<char> customPath;
    customPath.UnsafeInit("d:\\sounds\\xbox360\\", -1, 0);
    snd::CUSTOM_SOUND_PATH = customPath;

    FMOD_RESULT result = snd::SystemFMod->system->getSystemObject(&system);
    if (result) {
        if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"snd::SystemFMod->system->getSystemObject(&this->system)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2277, empty_string);
        const char *desc = FModErrorDesc(result, 0);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                             "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2277, result, desc);
        return false;
    }

    result = system->getDSPBufferSize(&minDelay, nullptr);
    if (result) {
        if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->getDSPBufferSize(&minDelay, NULL)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2279, empty_string);
        const char *desc = FModErrorDesc(result, 0);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                             "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2279, result, desc);
        return false;
    }

    minDelay *= 2;

    result = system->getSoftwareFormat(&outputRate, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (result) {
        if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->getSoftwareFormat(&outputRate, NULL, NULL, NULL, NULL, NULL)\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2282, empty_string);
        const char *desc = FModErrorDesc(result, 0);
        osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                             "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2282, result, desc);
        return false;
    }

    if (channels.allocated < 256)
        channels.Realloc(256);
    soundLists.Resize(15);

    // Filename locale suffix: empty for the default "en" locale, "_<locale>" otherwise (e.g.
    // "combat_dialog_fr.fsb" vs plain "combat_dialog.fsb").
    dsTSTRING<char> localeSuffix;
    localeSuffix.UnsafeInitEmpty();
    if (strcmp(snd::SOUND_LOCALE.CStr(), "en") != 0)
        operator_plus_dsTSTRING(&localeSuffix, "_", snd::SOUND_LOCALE);

    LoadTagParams();

    for (int i = 0; i < 15; ++i) {
        const SOUND_LIST_ENTRY &entry = SOUND_LIST_LST[i];
        dsTSTRING<char> filename;
        dsSPrintf(&filename, entry.filename, localeSuffix.CStr());
        soundLists[i].Init(entry.level, entry.name, filename.CStr(), entry.prepareStreams != 0);
    }

    return true;
}

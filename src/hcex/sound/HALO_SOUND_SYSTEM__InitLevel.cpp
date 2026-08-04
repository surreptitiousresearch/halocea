#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// fnmGetNameStr(dsTSTRING<char>*, const dsTSTRING<char>&) @ 0x82779608
// Strips the directory portion off `name`, returning just the filename (sret). Boundary --
// generic filename-utility subsystem, not reversed here.
extern dsTSTRING<char> *fnmGetNameStr(dsTSTRING<char> *result, const dsTSTRING<char> &name);

extern "C" int strcmp(const char *a, const char *b); // boundary — CRT

// HALO_SOUND_SYSTEM::InitLevel(dsTSTRING<char> const&) @ 0x836C03F0
// Marks the level initialized and preloads the FSB sound-list matching `fullName`'s (lower-
// cased) base filename. If no loaded list matches, recurses once on the literal "debug" list
// name so a debug-only level still gets a usable sound-list.
// DEVIATION: the disasm inlines the levelName-vs-baseName comparison as a raw pointer-equal-or-
// byte-loop (a manual reimplementation of dsTSTRING::operator==), and separately a manual
// byte-loop against the literal "debug" instead of a CRT strcmp call. Reproduced here using
// dsTSTRING::operator==() and strcmp() directly since both already exist and are semantically
// identical -- no behavior differs, only how the comparison is spelled.
void HALO_SOUND_SYSTEM::InitLevel(const dsTSTRING<char> &fullName)
{
    isLevelInited = true;

    dsTSTRING<char> baseName;
    fnmGetNameStr(&baseName, fullName);
    baseName.ToLower();

    int i;
    bool found = false;
    for (i = 0; i < soundLists.nElem; ++i) {
        if (soundLists[i].levelName == baseName) {
            found = true;
            break;
        }
    }

    if (found) {
        soundLists[i].Preload();
    } else if (strcmp(baseName.CStr(), "debug") != 0) {
        dsTSTRING<char> debugName;
        debugName.UnsafeInit("debug", -1, 0);
        InitLevel(debugName);
    }
}

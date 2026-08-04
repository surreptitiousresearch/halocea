#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/fnm/fnm.h"
#include <string.h>

// 0x836C03D8 — snd::SYSTEM_CUSTOM::InitLevel override: mark the level as inited, then find the
// FSB sound list whose stashed level name matches this map's file name (lower-cased, path/ext
// stripped) and Preload it. If no list matches, fall back to preloading the shared "debug" list
// (unless we already are the "debug" pass, avoiding infinite recursion).
void HALO_SOUND_SYSTEM::InitLevel(const dsTSTRING<char> &fullName)
{
    this->isLevelInited = true;

    dsTSTRING<char> name = fnmGetNameStr(fullName);
    name.ToLower();

    for (int i = 0; i < this->soundLists.nElem; ++i)
    {
        if (name == this->soundLists[i].levelName)
        {
            this->soundLists[i].Preload();
            return;
        }
    }

    // No per-level list — fall back to "debug", but not if this already is the debug pass.
    if (strcmp(name.CStr(), "debug") != 0)
    {
        dsTSTRING<char> debugName("debug");
        this->InitLevel(debugName);
    }
}

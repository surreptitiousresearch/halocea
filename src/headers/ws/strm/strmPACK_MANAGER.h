#pragma once
#include "strmPACK_BASE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
// ws-engine strm (streaming): owns the set of open strmPACK_BASE containers and resolves which
// one (if any) can serve a requested resource file. DB-verified layout (types_members
// strmPACK_MANAGER): packs@0 (dsVECTOR<strmPACK_BASE*,8>) — size 20.

typedef struct strmPACK_MANAGER {
    dsVECTOR<strmPACK_BASE *, 8> packs; // 0x00

    strmPACK_MANAGER();  // 0x828F1E20 — boundary, not decompiled here
    ~strmPACK_MANAGER();

    void AddPack(strmPACK_BASE *pack); // 0x828F1FF0-family — boundary, not decompiled here

    // 0x828F1EF0 — linear scan over `packs` for the first one whose CanOpenFile() accepts
    // (resName, pakId); on a hit, writes it out to *strmFile and returns true.
    bool CanOpenResourceFile(const dsSTRID &resName, int pakId, bool &cached, strmPACK_BASE **strmFile);
} strmPACK_MANAGER;

extern strmPACK_MANAGER strmPackMng; // ?strmPackMng@@3VstrmPACK_MANAGER@@A — global instance

#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: base of a brain's sound module (aiMIND::snd pointee). Only the per-spawn init
// hook reached by aiMIND::ProcessINITLive is declared; the full layout/bodies are the next
// frontier.  boundary.

struct aiSND_BASE_vtbl;

struct aiSND_BASE {
    aiSND_BASE_vtbl *__vftable; // 0x00

    // vtbl — per-spawn init hook. Used by aiMIND::ProcessINITLive.  boundary.
    void ProcessInit();
    // vtbl 0x04 — per-frame sound update. Used by aiMIND::ProcessFrameBeforeBody.  boundary.
    void ProcessFrame();

    // vtbl (?BuildExclamationName@aiSND_BASE@@UAA?AVdsSTRID@@ABV2@ABV?$dsTSTRING@D@@1@Z) — compose the
    // resolved sound id for exclamation `exID`, wrapped in `prefix`/`postfix`. Used by
    // aiBASS::InternalPlayEx.  boundary.
    dsSTRID BuildExclamationName(const dsSTRID &exID, const dsTSTRING<char> &prefix,
                                 const dsTSTRING<char> &postfix);
    // vtbl (?IsExclamationAccepted@aiSND_BASE@@UAA_NABVdsSTRID@@@Z) — whether this brain will let
    // exclamation `exID` play right now. Used by aiBASS::InternalPlayEx.  boundary.
    bool IsExclamationAccepted(const dsSTRID &exID);
};

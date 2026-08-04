#pragma once
#include "../nav/navPATH.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"

// ws-engine ai08 — a candidate (brain, path, entry-point) tuple used while inviting AIs to a
// smart-object. DB-verified layout (types_members aiBRAIN_PATH) — size 36:
//   ai@0, path@4 (navPATH), entryPointInfo@28 (dsPAIR<dsSTRID,dsSTRID>).

struct aiBRAIN; // ai08  boundary

struct aiBRAIN_PATH {
    aiBRAIN                 *ai;             // 0x00
    navPATH                  path;           // 0x04
    dsPAIR<dsSTRID, dsSTRID> entryPointInfo; // 0x1C

    aiBRAIN_PATH &operator=(const aiBRAIN_PATH &other); // reversed elsewhere  boundary
};

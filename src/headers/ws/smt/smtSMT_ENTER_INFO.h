#pragma once
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"

// ws-engine smt subsystem — one smart-object entry-point candidate.
// DB-verified layout (types_members smtSMT_ENTER_INFO) — size 12:
//   placeEnter@0 (gmPLACE_CHECKER*), info@4 (dsPAIR<dsSTRID,dsSTRID>).

struct gmPLACE_CHECKER; // gm subsystem — placement/entry checker (has ::trk entTRACKER)  boundary

struct smtSMT_ENTER_INFO {
    const gmPLACE_CHECKER   *placeEnter; // 0x00
    dsPAIR<dsSTRID, dsSTRID> info;       // 0x04
};

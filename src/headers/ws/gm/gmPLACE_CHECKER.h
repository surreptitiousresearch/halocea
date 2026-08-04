#pragma once
#include "../ent/entTRACKER.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsTSTRING.h"

// ws-engine gm subsystem — placement / smart-object entry checker. ref-verified layout
// (headers_ref gmPLACE_CHECKER.h) — size 0x144. Reads the tracked entry/obstacle points and the
// reach/angle gates used when validating an AI's approach to a smart-object entry point.

struct gmPLACE_CHECKER_vtbl;

struct gmPLACE_CHECKER {
    gmPLACE_CHECKER_vtbl *__vftable;      // 0x00
    entTRACKER  trk;                      // 0x04 entry-point tracker
    entTRACKER  trkObstacle1;             // 0x64
    entTRACKER  trkObstacle2;             // 0xC4
    apSTATE_T<unsigned char> state;       // 0x124
    float       distReach;                // 0x128
    float       angleReach;               // 0x12C
    float       angleSegment;             // 0x130
    float       distAngleSegmentIgnore;   // 0x134
    dsTSTRING<char> nameStartPoint;       // 0x138
    dsTSTRING<char> nameObstaclePoint1;   // 0x13C
    dsTSTRING<char> nameObstaclePoint2;   // 0x140
};

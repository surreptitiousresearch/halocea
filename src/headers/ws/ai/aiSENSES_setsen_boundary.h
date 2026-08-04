#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

// ws-engine ai08 — interned SetSenses selector ids and the "enable all alerts" tunable key,
// referenced by aiBRAIN::implSetSenses / aiISA::SetPar. Defined (dynamically initialised) in the
// ai08 translation unit; declared here as externs for the reversed consumers. boundary.

extern const dsSTRID AI_SETSEN_IDLE;
extern const dsSTRID AI_SETSEN_DEFAULT;
extern const dsSTRID AI_SETSEN_BATTLE;
extern const dsSTRID AI_SETSEN_DISABLED1;
extern const dsSTRID AI_SETSEN_DISABLED2;

extern const dsTSTRING<char> enableAllAlerts;

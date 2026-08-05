#pragma once
#include "../fsm/fsmHO_INFO.h"
#include "../prop/propENTCONTAINER.h"
// ws-engine use-subsystem — the per-body usage processor (hideout/ITRC/context driver). Only the
// fields the aiBODY methods reach (hoInfo, isLocked) are typed; the rest are kept as byte-accurate
// opaque padding keyed to the DB member offsets (types_members usePROCESSOR, size 0x220).
// Derives propENTCONTAINER per DB (base@0, 80B).

struct smtSMART_OBJ_BASE; // smt — smart object   boundary (ptr)

struct usePROCESSOR : propENTCONTAINER {
    fsmHO_INFO    hoInfo;                  // 0x050 (80) hideout-transform info
    /* 0x064 (100) owner..frameCurPropList, opaque. 428 bytes, not 408: the DB's detailed rows run
     * owner@100 through frameCurPropList@480 (48 bytes) and end at 528 = 0x210, which is where
     * isLocked starts. The short array put every field below here 20 bytes early. */
    unsigned char _mid[428];
    bool          isLocked;               // 0x210 (528) locked by mind (0x400000 mind flag)
    bool          isLockedExt;            // 0x211 (529)
    unsigned char _pad212[2];             // 0x212
    float         timerPrediction;        // 0x214 (532)
    float         accumDist;              // 0x218 (536)
    smtSMART_OBJ_BASE *pSmtObjCur;        // 0x21C (540) currently-attached smart object
};

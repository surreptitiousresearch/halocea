#pragma once
#include "bhpPARAMS.h"
#include "../ds/dsSTRID.h"
#include "bhvPATH_boundary.h" // bhvPATH_UseHOFn (useHOPath member-fn-ptr type)
#include "../ai/aiSPD.h"
#include "../ai/aiRETICLE.h"

// ws-engine bhp subsystem — path behavior-parameter block.
// ref/DB-verified layout (types_members bhpPATH_PAR) — size 0xB0. navPATH pathData stays a
// byte-accurate opaque placeholder (dtor runs at scope end; body boundary).

struct navPATH;   // ai/aiCHASER.h — path data     boundary (pointer views only)

struct bhpPATH_PAR : bhpPARAMS {
    // navPATH pathData @0x28 (0x18 bytes) — dtor navPATH::~navPATH runs at scope end. boundary blob.
    unsigned char pathData[0x18];              // 0x28
    aiSPD         speed;                       // 0x40
    int           idxNextDetectSpeed;          // 0x74
    dsSTRID       speedDesc;                    // 0x78
    int           isLoop;                      // 0x7C
    bool          isLastWPDetectionEnabled;    // 0x80
    int           isShoot;                     // 0x84
    int           isNeedAssoc;                 // 0x88
    bool          isImportant;                 // 0x8C
    bool          doNotifyNSChange;            // 0x8D
    bhvPATH_UseHOFn useHOPath;                 // 0x90 HO-selection predicate (member-fn-ptr)
    // The shipped slot is 16 bytes (MSVC generic pointer-to-member; IDA types it __int128), but a
    // host compiler that already has bhvPATH complete emits the 4-byte single-inheritance form.
    // Without this filler rtc and isLastWPHOPos sit 12 bytes early and the block sizes 164 vs 176.
    // See the SIZE CAVEAT in bhvPATH_boundary.h for why the member type is not widened instead.
    unsigned char _pad_useHOPath[16 - sizeof(bhvPATH_UseHOFn)];
    aiRETICLE     rtc;                         // 0xA0
    bool          isLastWPHOPos;               // 0xAC

    bhpPATH_PAR();
    void ParseSpeed(struct aiBRAIN *brain, dsTSTRING<char> *speedName);

    // ?PreparePathToHO@bhpPATH_PAR@@... — finalise this path-param block for a move-to-hideout
    // (marks the trailing waypoint as an HO approach). Used by aiBRAIN::implMoveToHO.  boundary.
    void PreparePathToHO();
};
